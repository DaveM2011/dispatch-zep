<?php
define("BASEDIR", dirname(__DIR__) . "/");
define("DATADIR", BASEDIR . "data/");
define('CHUNK_SIZE', 1024*1024);
define('ALLOWED_EXTS', ["exe", "zip", "iwd"]);

$redis = new \Redis();
$redis->connect("127.0.0.1", 6379);
$redis->auth('l8VjnR80ZN%qNXNJ!lyD');

function dir_from_access_token($token) {
    $privkey_res = openssl_pkey_get_private("file://" . BASEDIR . "priv.key");
    if ($privkey_res === false) return null;
    openssl_private_decrypt($token, $encrypted, $privkey_res);
    var_dump($encrypted);
}

function generate_link($file) {
    global $redis;
    $publickey_res = openssl_pkey_get_public("file://" . BASEDIR . "public.key");
    if ($publickey_res === false) return null;
    openssl_public_encrypt($file, $encrypted, $publickey_res);
    $key = hash_hmac("ripemd160", $file, $file);
    $redis->hset("links", $key, $file);
    return $key;
}

function readfile_chunked($filename, $retbytes = TRUE) {
  $buffer = "";
  $cnt = 0;
  $handle = fopen($filename, "rb");
  if($handle === false) {
    return false;
  }
  while(!feof($handle)) {
    $buffer = fread($handle, CHUNK_SIZE);
    echo $buffer;
    ob_flush();
    flush();
    if($retbytes) {
      $cnt += strlen($buffer);
    }
  }
  $status = fclose($handle);
  if($retbytes && $status) {
    return $cnt; // return num. bytes delivered like readfile() does.
  }
  return $status;
}

function send_file($file) {
	if(ini_get('zlib.output_compression')) {ini_set('zlib.output_compression', 'Off');}
	switch(strtolower(substr(strrchr($file, '.'), 1))) {
		case 'pdf': $mime = 'application/pdf'; break;
		case 'zip': $mime = 'application/zip'; break;
		case 'jpeg': case 'jpg': $mime = 'image/jpg'; break;
		case 'gif': $mime = 'image/gif'; break;
		case 'png': $mime = 'image/png'; break;
		default: $mime = 'application/octet-stream';
	}
	header('Pragma: public');//required
	header('Expires: 0');//no cache
	header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
	header('Last-Modified: '.gmdate('D, d M Y H:i:s', filemtime($file)).' GMT');
	header('Cache-Control: private',false);
	header('Content-Type: '.$mime);
	header('Content-Disposition: attachment; filename="'.basename($file).'"');
	header('Content-Transfer-Encoding: binary');
	header('Content-Length: '.filesize($file));//provide file size
	header('Connection: close');
	readfile_chunked($file);
	exit;
}

function del($file) {
	global $files_path;
	if(!file_exists($files_path.$file)) out(array('error' => 'File not found'));
	unlink($files_path.$file);
	out(array('ok' => 'File deleted'));
}

try {
    $app = new Dispatch\Dispatch();
    session_start();
    if (isset($_GET["access_token"])) {
        $privkey_res = openssl_pkey_get_private("file://../priv.key");
        if ($privkey_res === false) {
            echo "priv_key_error";
            exit;
        }
        openssl_private_decrypt(hex2bin($_GET["access_token"]), $decrypted, $privkey_res);
        $_SESSION["access_token"] = $_GET["access_token"];
        $_SESSION["access_id"] = $decrypted;
        $redirect = $app->redirect("/");
        $redirect->render();
        exit;
    }
    $authorized = isset($_SESSION["access_token"]) ? $_SESSION["access_token"] : false;
    $app->route("POST", "/upload", function () use ($app, $authorized) {
        if (!count($_FILES) || !$authorized) {
            return $app->response(json_encode(["error" => $authorized ? "no_files" : "not_authorized"]), 400, ["content-type" => "application/json"]);
        }
        $dir = DATADIR . $_SESSION["access_id"] . "/";
        if (!is_dir($dir)) {
            mkdir($dir);
        }
        $json = [];
        foreach($_FILES as $file) {
            $ext = pathinfo($file["name"], PATHINFO_EXTENSION);
            if(array_search($ext, ALLOWED_EXTS) !== -1) {
                $sanitized = preg_replace("/[^a-zA-Z0-9-_\.]/", "", basename($file["name"]));
                if(file_exists($dir.$sanitized)) {
                    $json["result"][] = ["file" => $sanitized, "error" => "file_exits"];
                    continue;
                }
                if(move_uploaded_file($file["tmp_name"], $dir.$sanitized)) {
                    //$vcheck = vcheck2($sanitized);
                    //var_dump($vcheck);
                    $json["result"][] = ["file" => $sanitized];
                } else {
                    unlink($file["tmp_name"]);
                    $json["result"][] = ["file" => $sanitized, "error" => "Error: Moving file"];
                }
                continue;
            }
            $json["result"][] = ["file" => basename($file["name"]), "error" => "Error: Invalid extension"];
        }
        return $app->response(json_encode($json), 200, ["content-type" => "application/json"]);
    });
    $app->route("POST", '/manage', function () use ($app, $authorized) {
        if (!isset($_GET["p"]) || !$authorized) {
            return $app->response(json_encode(["error" => $authorized ? "invalid_request" : "not_authorized"]), 400, ["content-type" => "application/json"]);
        }
        $json = [];
        $dir = DATADIR . $_SESSION["access_id"] . "/";
        if (!is_dir($dir)) {
            mkdir($dir);
        }
        $p = $_GET["p"];
        switch ($p) {
            case "link":
                $file = isset($_GET["f"]) ? $_GET["f"] : null;
                if (!$file) {
                    $json["error"] = "no_file";
                    break;
                }
                $json["data"] = generate_link($file);
                break;
            case "del":
                $file = isset($_GET["f"]) ? $_GET["f"] : null;
                if (!$file) {
                    $json["error"] = "no_file";
                    break;
                }
                $json["data"] = generate_link($file);
                break;
            default:
                $files = array_filter(array_slice(scandir($dir),2),function($a)use($dir){return !is_dir($dir.$a);});
                sort($files);
                $json["data"] = $files;
                break;
        }
        return $app->response(json_encode($json), 200, ["content-type" => "application/json"]);
    });
    $app->route("GET", '/books/:id', function ($args) use ($app, $authorized) {
        $json = json_encode($args);
        return $app->response($json, 200, ["content-type" => "application/json"]);
    });
    $app->route("GET", "/:hash", function ($args) use ($app, $authorized) {
        global $redis;
        $dir = DATADIR;
        if(!isset($args["hash"])) {
            echo "file_hash_missing";
            exit;
        }
        if(!$redis->hexists("links", $args["hash"])) {
            echo "file_not_found";
            exit;
        }
        $file = $redis->hget("links", $args["hash"]);
        $ref = isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : false;
        $req = isset($_GET['tkn']) ? $_GET['tkn'] : true;
        //if(preg_match("/zombiemodding\.com|zm-dev\.com/", $ref) && $req) {
            if(is_file($dir.$file)) {
                send_file($dir.$file);
            }
            echo "file_not_found";
        //} else {
            //echo "Invalid referer";
        //}
        exit;
    });
    $app->route("GET", "/", function () use ($app, $authorized) {
        $html = $app->phtml(__DIR__ . '/views/theme', ['page' => 'home', 'authorized' => $authorized]);
        return $app->response($html);
    });
    $app->dispatch();
} catch (Exception $e) {
    throw $e;
}
?>