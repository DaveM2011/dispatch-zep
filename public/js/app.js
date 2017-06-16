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
                m("a", {href: "#!/upload"}, "Upload"),
                m("a", {href: "#!/manage"}, "Manage")
            ]
        }
    }
    const Upload = {
        view: vnode => m(".wrap", [
            m(".tc", "Max file size: " + maxFileSize),
            m(".dnd"),
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