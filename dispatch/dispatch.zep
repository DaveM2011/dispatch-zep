namespace Dispatch;
use Exception;

class Dispatch {
    private static _inst;
    private routes = [];
    private vars = [];
    public function __construct() {
        let self::_inst = this;
    }
    public static function context() {
        return self::_inst;
    }
    // dispatch sapi request against routes context
    public function dispatch() {
        var args, method, path, responder;
        let args = func_get_args();
        let method = strtoupper(_SERVER["REQUEST_METHOD"]);
        let path = "/" . trim(parse_url(_SERVER["REQUEST_URI"], PHP_URL_PATH), "/");
        // post method override
        if method == "POST" {
            if isset _SERVER["HTTP_X_HTTP_METHOD_OVERRIDE"] {
                let method = strtoupper(_SERVER["HTTP_X_HTTP_METHOD_OVERRIDE"]);
            } elseif isset _POST["_method"] {
                let method = strtoupper(_POST["_method"]);
            }
        }
        let responder = call_user_func_array([this, "serve"], array_merge([this->routes, method, path], args));
        call_user_func([responder, "render"]);
    }
    // creates an action and puts it into the routes stack
    public function route(method, path, callable func) {
        array_push(this->routes, this->action(method, path, func));
    }
    // creates a route handler
    public function action(method, path, callable func) {
        return create_instance_params("\\Dispatch\\Action", [method, path, func]);
    }
    // performs a lookup against actions for method + path
    public function match(array actions, method, path) {
        var cpath, action, match;
        let cpath = "/" . trim(rawurldecode(parse_url(path, PHP_URL_PATH)), "/");
        // test method + path against route handlers
        for action in actions {
            let match = call_user_func_array([action, "match"], [method, cpath]);
            if match {
                return match;
            }
        }
        return [function () { return Dispatch::context()->response("", 404, []); }, null];
    }
    // creates standard response
    public function response(body, int code = 200, array headers = []) {
        return create_instance_params("\\Dispatch\\Response", [body, code, headers]);
    }
    // creates redirect response
    public function redirect(location, int code = 302) {
        return create_instance_params("\\Dispatch\\Response", ["", code, ["location": location]]);
    }
    // dispatches method + path against route stack
    public function serve(array actions, method, path) {
        var args, pair, caps;
        let args = array_slice(func_get_args(), 3);
        let pair = this->match(actions, method, path);
        if fetch caps, pair[1] {
            array_unshift(args, caps);
            return call_user_func_array(pair[0], args);
        }
        return call_user_func_array(pair[0], args);
    }
    // renders request response to the output buffer (ref: zend-diactoros)
    public function render(body, int code = 200, array headers = []) {
        http_response_code(code);
        array_walk(headers, [this, "header"]);
        echo body;
    }
    public function header(value, key) {
        if !preg_match("/^[a-zA-Z0-9\'`#$%&*+.^_|~!-]+$/", key) {
            throw new \InvalidArgumentException("Invalid header name - {$key}");
        }
        var values, val;
        let values = is_array(value) ? value : [value];
        for val in values {
            if preg_match("#(?:(?:(?<!\r)\n)|(?:\r(?!\n))|(?:\r\n(?![ \t])))#", val) || preg_match("/[^\x09\x0a\x0d\x20-\x7E\x80-\xFE]/", val) {
                throw new \InvalidArgumentException("Invalid header value - {$val}");
            }
        }
        header(key . ": " . implode(",", values));
    }
    // creates an page-rendering action
    public function page(path, array vars = []) {
        return create_instance_params("\\Dispatch\\Response", [path, vars]);
    }
    // renders and returns the content of a template
    public function phtml(path, array vars = []) {
        ob_start();
        extract(this->vars, EXTR_SKIP);
        extract(vars, EXTR_SKIP);
        require path . ".phtml";
        return trim(ob_get_clean());
    }
}
