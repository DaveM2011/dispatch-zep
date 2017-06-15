<?php
try {
    $app = new Dispatch\Dispatch();
    $app->route("GET", "/", function () use ($app) {
        $html = $app->phtml(__DIR__ . '/views/theme', ['page' => 'home']);
        return $app->response($html);
    });
    $app->route("GET", '/books/:id', function ($args) use ($app) {
        $json = json_encode($args);
        return $app->response($json, 200, ["content-type" => "application/json"]);
    });
    $app->dispatch();
} catch (\Exception $e) {
    //var_dump($e);
    throw $e;
}
?>