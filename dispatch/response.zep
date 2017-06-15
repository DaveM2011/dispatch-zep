namespace Dispatch;

class Response {
    private body;
    private code;
    private headers;
    public function __construct(body, code, headers) {
        let this->body = body;
        let this->code = code;
        let this->headers = headers;
    }
    public function render() {
        Dispatch::context()->render(this->body, this->code, this->headers);
    }
    public function response() {
        return Dispatch::context()->response(Dispatch::context()->phtml(this->body, this->code));
    }
}