<?php

function convertIntToShortCode($id) {
    $chars = "123456789bcdfghjkmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
    $id = intval($id);
    if ($id < 1) {
        throw new Exception("The ID is not a valid integer");
    }
    $length = strlen($chars);
    // make sure length of available characters is at
    // least a reasonable minimum - there should be at
    // least 10 characters
    if ($length < 10) {
        throw new Exception("Length of chars is too small");
    }
    $code = "";
    while ($id > $length - 1) {
        // determine the value of the next higher character
        // in the short code should be and prepend
        $code = $chars[fmod($id, $length)] . $code;
        // reset $id to remaining value to be converted
        $id = floor($id / $length);
    }
    // remaining value of $id is less than the length of
    // self::$chars
    $code = $chars[$id] . $code;
    return $code;
}

$user = 3;
$publickey_res = openssl_pkey_get_public("file://../public.key");
if ($publickey_res === false) return null;
openssl_public_encrypt($user, $encrypted, $publickey_res);
$access_token = bin2hex($encrypted);
var_dump($user, convertIntToShortCode($user), $access_token);
header("Location: /?access_token=" . $access_token);
?>