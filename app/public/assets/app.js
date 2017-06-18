const AppInit = (function(maxFileSize, authorized, username) {

    delete window.AppInit;
    
    console.log(maxFileSize, authorized, username);

    const allowedExts = ["exe", "zip", "iwd", "mkv"]

    const copyToClipboard = text => {
        if (window.clipboardData && window.clipboardData.setData) {
            // IE specific code path to prevent textarea being shown while dialog is visible.
            return clipboardData.setData("Text", text);
        } else if (document.queryCommandSupported && document.queryCommandSupported("copy")) {
            var textarea = document.createElement("textarea");
            textarea.textContent = text;
            textarea.style.position = "fixed";  // Prevent scrolling to bottom of page in MS Edge.
            document.body.appendChild(textarea);
            textarea.select();
            try {
                return document.execCommand("copy");  // Security exception may be thrown by some browsers.
            } catch (ex) {
                console.warn("Copy to clipboard failed.", ex);
                return false;
            } finally {
                document.body.removeChild(textarea);
            }
        }
    }

    const bytesToSize = bytes => {
        if(bytes == 0) return '0 Byte';
        var k = 1000;
        var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];
        var i = Math.floor(Math.log(bytes) / Math.log(k));
        return (bytes / Math.pow(k, i)).toPrecision(3) + ' ' + sizes[i];
    }

    const throttle = (func, wait, options) => {
        var context, args, result;
        var timeout = null;
        var previous = 0;
        options || (options = {});
        var later = function() {
            previous = options.leading === false ? 0 : new Date().getTime();
            timeout = null;
            result = func.apply(context, args);
            context = args = null;
        }
        return function() {
            var now = new Date().getTime();
            if(!previous && options.leading === false) previous = now;
            var remaining = wait - (now - previous);
            context = this;
            args = arguments;
            if(remaining <= 0) {
                clearTimeout(timeout);
                timeout = null;
                previous = now;
                result = func.apply(context, args);
                context = args = null;
            } else if(!timeout && options.trailing !== false) {
                timeout = setTimeout(later, remaining);
            }
            return result;
        }
    }

    const Msg = {
        _timer: null,
        text: null,
        show: (text, timeout) => (Msg.text = text, Msg._timer = setTimeout(Msg.hide, timeout || 2000)),
        hide: () => (clearTimeout(Msg._timer), Msg._timer = null, Msg.text = null, m.redraw()),
        view: vnode => m("div.msg" + (Msg.text ? ".showmsg" : ""), m("div", Msg.text))
    }

    const File = {
        view: vnode => m(".item", [
            m(".main", [
                m(".name", {title: vnode.attrs.name}, vnode.attrs.name),
                m(".size", bytesToSize(vnode.attrs.size))
            ]),
            m(".action", [
                m(".link", {onclick: () => {copyToClipboard(location.origin + "/" + vnode.attrs.link), Msg.show("Link copied to clipboard") }}),
                m(".delete", {onclick: e => File.remove(vnode.attrs.name)}),
            ])
        ]),
        remove: name => {
            console.log(name);
            if (!name)
                return false;
            m.request({
                method: "POST",
                url: "/manage?p=del&f=" + name
            }).then(data => (Manage.load(), Msg.show("File removed"), data))
        }
    }

    const Layout = {
        view: vnode => {
            return m("main", [
                m("header", [
                    m(".logo"),
                    m(".brand", [
                        m(".title", "Zombie Modding"),
                        m(".sub", "Uploader")
                    ]),
                    username && m(".username", [
                        m("", "Hi " + username),
                        m("", "|"),
                        m("a[href=/logout]", "Logout")
                    ])
                ]),
                m("#content", [
                    vnode.attrs.hasNav ? [
                        m("nav", [
                            m("li", m("a", {class: vnode.attrs.active == "upload" ? "active" : "", href: "/", oncreate: m.route.link}, "Upload")),
                            m("li", m("a", {class: vnode.attrs.active == "manage" ? "active" : "", href: "/manage", oncreate: m.route.link}, "Manage"))
                        ])
                    ] : null,
                    vnode.children
                ]),
                m(Msg)
            ])
        }
    }

    const Uploaded = {
        view: vnode => {
            return [
                m("#uploaded", [
                    m(".helper", [
                        m(".icon", {class: vnode.attrs.ext}),
                        m("p", vnode.attrs.file + " has been uploaded"),
                        m("#linkInput", [
                            m("input", {onclick: e => e.target.select(), placeholder: "link", value: location.origin + "/" + vnode.attrs.link, readonly: ""}),
                            m(".link", {onclick: () => { copyToClipboard(location.origin + "/" + vnode.attrs.link), Msg.show("Link copied to clipboard") }})
                        ])
                    ])
                ])
            ]
        }
    }

    const NoAuth = {
        view: vnode => {
            return [
                m("#noAuth", [
                    m(".helper", [
                        m(".icon"),
                        m("p", [m("span", "You need to "), m("a[href='']", "login"), m("span"," to access the uploader")])
                    ])
                ])
            ]
        }
    }

    const Upload = {
        uploaded: null,
        error: null,
        current: null,
        view: vnode => {
            return vnode.state.uploaded && m(Uploaded, vnode.state.uploaded) || [
                m("#dropzone", {
                    ondragenter: e => {
                        e.stopPropagation();
                        e.preventDefault();
                        return false;
                    },
                    ondragover: e => {
                        e.stopPropagation();
                        e.preventDefault();
                        return false;
                    },
                    ondrop: e => {
                        e.stopPropagation();
                        e.preventDefault();
                        if(vnode.state.current) return false;
                        var dataTransfer = e.dataTransfer;
                        interval = null;
                        if(dataTransfer && dataTransfer.files && dataTransfer.files.length > 0) {
                            for(let i = 0; i < dataTransfer.files.length; i++) {
                                var file = dataTransfer.files[i];
                                var ext = file.name.substr(file.name.lastIndexOf('.') + 1);
                                if (allowedExts.indexOf(ext) == -1) continue;
                                //if (file.size > 536870912) continue;
                                if (file.size > 1073741824) continue;
                                var data = new FormData();
                                data.append('dlmodulef', file);
                                var received = 0, check = new Date().getTime(), bytesUploaded = 0, tracking = {};
                                var tfun = throttle(function(e){
                                    bytesUploaded = e.loaded || 0;
                                    if(bytesUploaded < 0) {
                                        bytesUploaded = 0;
                                    }
                                    if(bytesUploaded > e.total) {
                                        bytesUploaded = e.total;
                                    }
                                    var tickTime = new Date().getTime();
                                    if(!tracking.startTime) {
                                        tracking.startTime = new Date().getTime();
                                        tracking.lastTime = tracking.startTime;
                                        tracking.currentSpeed = 0;
                                        tracking.averageSpeed = 0;
                                        tracking.maxSpeed = 0;
                                        tracking.bytesUploaded = bytesUploaded;
                                    } else if(tracking.startTime > tickTime) {
                                        //this.debug("When backwards in time");
                                    } else {
                                        // Get time and deltas
                                        var now = new Date().getTime();
                                        //var lastTime = tracking.lastTime;
                                        var deltaTime = now - tracking.lastTime;
                                        var deltaBytes = bytesUploaded - tracking.bytesUploaded;
                                        if(deltaBytes === 0 || deltaTime === 0) {
                                            //return tracking;
                                        }
                                        // Update tracking object
                                        tracking.lastTime = now;
                                        tracking.bytesUploaded = bytesUploaded;
                                        // Calculate speeds
                                        tracking.currentSpeed = (deltaBytes * 8) / (deltaTime / 1000);
                                        tracking.averageSpeed = (tracking.bytesUploaded * 8) / ((now - tracking.startTime) / 1000);
                                        tracking.maxSpeed = tracking.currentSpeed > tracking.maxSpeed ? tracking.currentSpeed : tracking.maxSpeed;
                                    }
                                }, 1000);
                                
                                m.request({
                                    method: "POST",
                                    url: "/upload",
                                    data: data,
                                    config: xhr => {
                                        var upload = xhr.upload;
                                        upload.onprogress = function(e) {
                                            tfun(e);
                                            vnode.state.current = {
                                                loaded: bytesToSize(e.loaded),
                                                total: bytesToSize(e.total),
                                                progress: Math.round(e.loaded / e.total * 100),
                                                speed: {current: bytesToSize(tracking.currentSpeed), average: bytesToSize(tracking.averageSpeed), max: bytesToSize(tracking.maxSpeed)}
                                            }
                                            m.redraw();
                                        }
                                    }
                                }).then(data => (data.result[0] && (vnode.state.uploaded = data.result[0]), data))
                            }
                        }
                        return false;
                    }
                }, [
                    m(".info", [
                        vnode.state.current && m(".center.progress-radial.progress-" + vnode.state.current.progress, m(".overlay", vnode.state.current.progress + "%")) || m(".icon"),
                        vnode.state.current && m(".center", [
                            m("", "Sent: " + vnode.state.current.loaded + "/" + vnode.state.current.total),
                            m("", "Speed (current, average, max): " + vnode.state.current.speed.current + "/" + vnode.state.current.speed.average + "/" + vnode.state.current.speed.max)
                        ]),
                        m("span", "Max file size: " + maxFileSize)
                    ])
                ])
            ]
        }
    }
    const Manage = {
        files: [],
        oninit: vnode => Manage.load(),
        view: vnode => {
            return [
                m("#manage", [
                    vnode.state.files.length == 0 && m("li", {title: "No files"}),
                    vnode.state.files.map(a => m(File, a))
                ])
            ]
        },
        load: () => m.request({
            method: "POST",
            url: "/manage?p=list"
        })
        .then(result => {
            Manage.files = result.data
        })
    }

    m.route(document.getElementById("app"), authorized ? "/" : "/noauth", {
        "/": {onmatch: () => (authorized && Upload || NoAuth), render: vnode => m(Layout, {hasNav: true, active: "upload"}, vnode)},
        "/manage": {onmatch: () => (authorized && Manage || NoAuth), render: vnode => m(Layout, {hasNav: true, active: "manage"}, vnode)},
        "/uploaded": {onmatch: () => (authorized && Uploaded || NoAuth), render: vnode => m(Layout, vnode)},
        "/logout": {onmatch: () => window.location = "/logout"},
        "/noauth": {render: () => m(Layout, m(NoAuth))}
    })
})
