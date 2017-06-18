<?php

$user = json_encode([3, "BigDave"]);
$publickey_res = openssl_pkey_get_public("file://../public.key");
if ($publickey_res === false) return null;
openssl_public_encrypt($user, $encrypted, $publickey_res);
$access_token = bin2hex($encrypted);
header("Location: /?access_token=" . $access_token);
?>