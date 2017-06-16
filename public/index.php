<?php
define("BASEDIR", dirname(__DIR__) . "/");
define("DATADIR", BASEDIR);
define('CHUNK_SIZE', 1024*1024);

$redis = new \Redis();
$redis->connect("127.0.0.1", 6379);
$redis->auth('l8VjnR80ZN%qNXNJ!lyD');

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
        $_SESSION["access_token"] = $_GET["access_token"];
        $redirect = $app->redirect("/");
        $redirect->render();
        exit;
    }
    $authorized = isset($_SESSION["access_token"]) ? $_SESSION["access_token"] : false;
    $app->route("POST", "/upload", function () use ($app, $authorized) {
        $json = json_encode([]);
        return $app->response($json, 200, ["content-type" => "application/json"]);
    });
    $app->route("POST", '/manage', function () use ($app, $authorized) {
        if (!isset($_GET["p"]) || !$authorized) {
            return $app->response(json_encode(["error" => "invalid_request"]), 400, ["content-type" => "application/json"]);
        }
        $json = [];
        $dir = DATADIR;
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