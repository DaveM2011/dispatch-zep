const AppInit = (function(maxFileSize, token){
    delete window.AppInit;
    console.log(maxFileSize, token);
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
    const File = {
        //oninit: vnode => console.log(vnode),
        view: vnode => m(polythene.listTile, {title: vnode.attrs.name, secondary: {content: [
            m(polythene.button, {events: {onclick: File.getPublicLink.bind(vnode.attrs)}}, "Delete"),
            m(polythene.button, {events: {onclick: File.getPublicLink.bind(vnode.attrs)}}, "Get public link")
        ]}}),
        /*m(polythene.card, {
            content: [{
                primary: {
                    content: [
                        m("div", vnode.attrs.name),
                        m("a.pub-link", {onclick: File.getPublicLink.bind(vnode.attrs)}, "Get public link")
                    ]
                }}
            ]
        }),*/
        getPublicLink: function(e) {
            console.log(this)
            var name = this.name;
            if (!name) return false;
            polythene.dialog.show({
                tone: "dark",
                backdrop: true,
                modal: true,
                content: m(polythene.mdSpinner, {
                    permanent: true
                })
            }, {
                id: "publiclink"
            }).then(() => m.request({
                method: "POST",
                url: "/manage?p=link&f=" + name
            }).then(result => (polythene.dialog.show({
                tone: "dark",
                backdrop: true,
                class: "wrap-text",
                style: {
                    position: "relative",
                },
                footer: [
                    m(polythene.button, {
                        label: "Copy to clipboard",
                        events: {
                            onclick: () => (copyToClipboard(location.origin + "/" + result.data), polythene.dialog.hide({id: "publiclink"}), polythene.snackbar.show({tone: "light", title: "Link copied to clipboard", timeout: 2}, {position: "container"}))
                        }
                    }),
                    m(polythene.button, {
                        label: "Close",
                        events: {
                            onclick: () => polythene.dialog.hide({
                                id: "publiclink"
                            })
                        }
                    })
                ],
                content: [
                    m("textarea", {
                        onclick: e => e.target.select()
                    }, location.origin + "/" + result.data),
                    m("a", {
                        href: location.origin + "/" + result.data,
                        target: "_blank"
                    }, "Open in new tab")
                ]
            }, {
                id: "publiclink"
            }), result.data)));
            return false
        }
    }
    const UploadFile = (file, xhr) => ({

    })
    const Layout = {
        view: vnode => m("main", [
            m(polythene.toolbar, {tone: "dark"}, [
                m(polythene.button, {label: "Upload", url: {href: "/upload", oncreate: m.route.link}}),
                m(polythene.button, {label: "Manage", url: {href: "/manage", oncreate: m.route.link}}),
            ]),
            m("section", vnode.children),
            m(polythene.dialog, { id: "publiclink" }),
            m(polythene.snackbar)
        ])
    }
    const Home = {
        view: function() {
            return [
                
            ]
        }
    }
    const Upload = {
        oninit: vnode => {

        },
        view: vnode => m(".wrap", [
            m(".tc", "Max file size: " + maxFileSize),
            m(".dnd", {
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
                    var dataTransfer = e.dataTransfer;
                    interval = null;
                    if(dataTransfer && dataTransfer.files && dataTransfer.files.length > 0) {
                        for(var i = 0; i < dataTransfer.files.length; i++) {
                            var file = dataTransfer.files[i];
                            var xhr = new XMLHttpRequest();
                            var upload = xhr.upload;
                            xhr.onreadystatechange = function(){
                                if(xhr.readyState === 4) {
                                    if(xhr.status === 200) {
                                        //if($this.data('done')) $this.data('done').apply($this, [{id: uuid, progress: 100, response: xhr.responseText}]);
                                    } else {
                                        //if($this.data('error')) $this.data('error').apply($this, [{id: uuid, progress: 100, response: xhr.responseText}]);
                                    }
                                }
                            }
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
                                }
                            }, 1000);
                            upload.onprogress = function(e) {
                                tfun(e);
                                //if($this.data('progress')) $this.data('progress').apply($this, [{id: uuid, progress: Math.round(e.loaded / e.total * 100), speed: {current: tracking.currentSpeed, average: tracking.averageSpeed}}]);
                            }
                            var uuid = "";//methods.genId();
                            xhr.open('POST', '/upload', true);
                            xhr.setRequestHeader('test', file.fileName);
                            var data = new FormData();
                            data.append('dlmodulef', file);
                            data.append('ns', 'yes');
                            xhr.send(data);
                            var item = {
                                id: uuid,
                                file: file
                            }
                            //if($this.data('drop')) $this.data('drop').apply($this, [item, xhr]);
                        }
                    }
                    return false;
                }
            }),
            m("a.hint--bottom.hint--rounded.hide", {
                href: "/?ztmpl=" + new Date().getTime(),
                "data-hint": "Used to upload map in zip file, must contain (token file, url file & one folder with map files inside)"
            }, "Map upload zip"),
        ])
    }
    const Manage = {
        files: [],
        oninit: vnode => m.request({
            method: "POST",
            url: "/manage?p=list",
            withCredentials: true,
        })
        .then(result => {
            Manage.files = result.data
        }),
        view: vnode => m(polythene.list, [
            vnode.state.files.length == 0 && m(polythene.listTile, {title: "No files"}),
            vnode.state.files.map(a => m(File, {name: a}))
        ])
    }
    var root = document.getElementById("wrap");
    m.route(root, "/", {
        "/": {render: () => m(Layout, m(Home))},
        "/upload": {render: () => m(Layout, m(Upload))},
        "/manage": {render: () => m(Layout, m(Manage))}
    })
})