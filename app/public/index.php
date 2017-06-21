<?php
define("BASEDIR", dirname(__DIR__) . "/");
define("DATADIR", BASEDIR . "data/");
define('CHUNK_SIZE', 1024*1024);
define('HMAC_KEY', "G7^j26fhue)hgw1197");

$redis = new \Redis();
$redis->connect("127.0.0.1", 6379);
$redis->auth('l8VjnR80ZN%qNXNJ!lyD');

function generate_link($file) {
    global $redis;
    //$publickey_res = openssl_pkey_get_public("file://" . BASEDIR . "public.key");
    //if ($publickey_res === false) return null;
    //openssl_public_encrypt($file, $encrypted, $publickey_res);
    $key = hash_hmac("ripemd160", $file, HMAC_KEY);
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

function check_auth($app) {
    if (isset($_GET["access_token"])) {
        $privkey_res = openssl_pkey_get_private("file://../priv.key");
        if ($privkey_res === false) {
            echo "priv_key_error";
            exit;
        }
        openssl_private_decrypt(hex2bin($_GET["access_token"]), $decrypted, $privkey_res);
        $_SESSION["user"] = json_decode($decrypted);
        //$_SESSION["access_token"] = $_GET["access_token"];
        //$_SESSION["access_id"] = $decrypted;
        $redirect = $app->redirect("/");
        $redirect->render();
        exit;
    }
    return isset($_SESSION["user"]) ? true : false;
}

try {
    $app = new Dispatch\Dispatch();
    session_start();
    $authorized = check_auth($app);
    $app->route("POST", "/upload", function () use ($app, $authorized, $redis) {
        if (!count($_FILES) || !$authorized) {
            return $app->response(json_encode(["error" => $authorized ? "no_files" : "not_authorized"]), 401, ["content-type" => "application/json"]);
        }
        $dir = $_SESSION["user"][0] . "/";
        if (!is_dir(DATADIR . $dir)) {
            mkdir(DATADIR . $dir);
        }
        $status = 200;
        $json = [];
        foreach($_FILES as $file) {
            $ext = pathinfo($file["name"], PATHINFO_EXTENSION);
            $json["debug"] = [$ext, array_search($ext, ["exe", "zip", "iwd", "mkv"])];
            if(array_search($ext, ["exe", "zip", "iwd", "mkv"]) !== false) {
                $sanitized = preg_replace("/[^a-zA-Z0-9-_\.]/", "", basename($file["name"]));
                if(file_exists(DATADIR . $dir . $sanitized)) {
                    unlink(DATADIR . $dir . $sanitized);
                    //$status = 409;
                    //$json["result"][] = ["file" => $sanitized, "error" => "file_exits"];
                    //continue;
                }
                if(move_uploaded_file($file["tmp_name"], DATADIR . $dir . $sanitized)) {
                    //$vcheck = vcheck2($sanitized);
                    //var_dump($vcheck);
                    $key = hash_hmac("ripemd160", $dir . $sanitized, HMAC_KEY);
                    $redis->hset("links", $key, $dir . $sanitized);
                    $json["result"][] = ["file" => $sanitized, "ext" => $ext, "link" => $key];
                } else {
                    unlink($file["tmp_name"]);
                    $status = 412;
                    $json["result"][] = ["file" => $sanitized, "error" => "Error: Moving file"];
                }
                continue;
            }
            $status = 415;
            $json["result"][] = ["file" => basename($file["name"]), "error" => "Error: Invalid extension"];
        }
        return $app->response(json_encode($json), $status, ["content-type" => "application/json"]);
    });
    $app->route("POST", '/manage', function () use ($app, $authorized, $redis) {
        if (!isset($_GET["p"]) || !$authorized) {
            return $app->response(json_encode(["error" => $authorized ? "invalid_request" : "not_authorized"]), 401, ["content-type" => "application/json"]);
        }
        $json = [];
        $dir = $_SESSION["user"][0] . "/";
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
                $key = hash_hmac("ripemd160", $dir . $file, HMAC_KEY);
                $redis->hdel("links", $key);
                $json["data"] = unlink(DATADIR . $dir . $file);
                break;
            default:
                $found = array_filter(array_slice(scandir(DATADIR . $dir),2), function($a) use ($dir){
                    return !is_dir(DATADIR . $dir . $a);
                });
                $files = [];
                foreach($found as $file) {
                    $files[] = ["name" => $file, "size" => filesize(DATADIR . $dir . $file), "link" => hash_hmac("ripemd160", $dir . $file, HMAC_KEY)];
                }
                sort($files);
                $json["data"] = $files;
                break;
        }
        return $app->response(json_encode($json), 200, ["content-type" => "application/json"]);
    });
    $app->route("GET", '/logout', function () use ($app, $authorized) {
        unset($_SESSION["user"]);
        //unset($_SESSION["access_id"]);
        return $app->redirect("/");
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
        $ref = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : false;
        $req = isset($_GET["tkn"]) ? $_GET["tkn"] : true;
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
    $app->route("GET", "/f/:file", function ($args) use ($app, $authorized) {
        global $redis;
        $dir = DATADIR . "old/";
        if(!isset($args["file"])) {
            echo "file_name_missing";
            exit;
        }
        $ref = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : false;
        $req = isset($_GET["tkn"]) ? $_GET["tkn"] : true;
        if(preg_match("/zombiemodding\.com|zm-dev\.com/", $ref) && $req) {
            if(is_file($dir.$args["file"])) {
                send_file($dir.$args["file"]);
            }
            echo "file_not_found";
        } else {
            echo "invalid_referer";
        }
        exit;
    });
    $app->route("GET", "/", function () use ($app, $authorized) {
        $html = $app->phtml(__DIR__ . "/views/theme", ["page" => "home", "authorized" => $authorized, "username" => $authorized ? $_SESSION["user"][1] : null]);
        return $app->response($html);
    });
    $app->dispatch();
} catch (Exception $e) {
    throw $e;
}
?>
