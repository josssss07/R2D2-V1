let gateway = `ws://${window.location.hostname}/ws`;
let websocket;
let cmdMap = {};
const inputs = [...document.querySelectorAll("button")];

function onLoad(){
    initWebSocket();
}
window.addEventListener('load', onLoad);

function sendCmd(arg){
    websocket.send(arg);
    console.log(arg);
}

function stop(){
    websocket.send("s");
    console.log("s");
}

function initWebSocket(){
    console.log("Trying to open a websocket connection:");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(){
    console.log("Connection etablished: ");
    inputs.forEach(elem => {
        elem.onmousedown = sendCmd.bind(this, elem.id);
        elem.ontouchstart = sendCmd.bind(this, elem.id);
        if (elem.id == "l" || elem.id == "r"){    
            elem.onmouseup = sendCmd.bind(this, "s");
            elem.onmouseout = sendCmd.bind(this, "s");
            elem.ontouchmove = sendCmd.bind(this, "s");
            elem.ontouchend = sendCmd.bind(this, "s");
        }
        else{
            elem.onmouseup = stop;
            elem.onmouseout = stop;
            elem.ontouchmove = stop;
            elem.ontouchend = stop;
        }
    })
}


function onClose(){
    console.log("Connection lost: ");
    setTimeout(initWebSocket, 5000);
}

function onMessage(event){
    console.log("Message received: ");
    console.log
}

// onOpen()