namespace Dispatch;

class Action {
    private method;
    private path;
    private func;
    public function __construct(method, path, func) {
        let this->method = method;
        let this->path = path;
        let this->func = func;
    }
    public function match(method, path) {
        var rexp, caps;
        let rexp = preg_replace("@:(\w+)@", "(?<\1>[^/]+)", this->path);
        if method != this->method || !preg_match("@^" . rexp . "$@", path, caps) {
            return null;
        }
        return [this->func, array_slice(caps, 1)];
    }
}
