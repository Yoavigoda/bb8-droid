const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>BB-8 Control</title>
<style>
:root{--bg:#0e1013;--panel:#171a20;--line:#262b34;--text:#e8e6e3;--dim:#8a8f99;--amber:#f0932b}
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent}
body{margin:0;padding:16px;min-height:100vh;background:var(--bg);color:var(--text);
 font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;
 user-select:none;-webkit-user-select:none;touch-action:manipulation;
 display:flex;flex-direction:column;gap:14px}
header{display:flex;align-items:center;justify-content:space-between}
h1{font-size:17px;font-weight:600;margin:0}
#dot{width:9px;height:9px;border-radius:50%;background:var(--dim);transition:background .2s}
#dot.ok{background:var(--amber)}
.readout{background:var(--panel);border:1px solid var(--line);border-radius:14px;
 padding:14px 16px;display:flex;align-items:baseline;justify-content:space-between}
.label{font-size:12px;color:var(--dim);letter-spacing:.08em;text-transform:uppercase}
#count{font:600 26px/1 ui-monospace,Menlo,monospace;font-variant-numeric:tabular-nums}
.pad{display:flex;flex-direction:column;gap:12px;flex:1}
button{font:600 18px/1 inherit;color:var(--text);background:var(--panel);
 border:1px solid var(--line);border-radius:16px;width:100%;flex:1;min-height:84px;
 display:flex;align-items:center;justify-content:center;gap:10px;
 transition:transform .06s,background .12s,border-color .12s}
button:active{transform:scale(.985)}
.drive:active{background:#2a2016;border-color:var(--amber);color:var(--amber)}
#stopBtn{background:#241417;border-color:#3a1f22;color:#ff8a80;min-height:72px}
#stopBtn:active{background:#e0483c;border-color:#e0483c;color:#fff}
.arrow{font-size:22px}
.hint{text-align:center;font-size:12px;color:var(--dim);margin:0}
</style>
</head>
<body>

<header>
  <h1>BB-8 Droid</h1>
  <div id="dot"></div>
</header>

<div class="readout">
  <span class="label">Encoder A</span>
  <span id="count">--</span>
</div>

<div class="pad">
  <button class="drive" data-go="/forward"><span class="arrow">&#9650;</span> Forward</button>
  <button id="stopBtn">STOP</button>
  <button class="drive" data-go="/backword"><span class="arrow">&#9660;</span> Backward</button>
</div>

<p class="hint">Hold to drive &mdash; release to stop</p>

<script>
var dot = document.getElementById('dot');
var countEl = document.getElementById('count');

function send(route){
  return fetch(route)
    .then(function(r){ dot.classList.toggle('ok', r.ok); })
    .catch(function(){ dot.classList.remove('ok'); });
}

document.querySelectorAll('.drive').forEach(function(b){
  b.addEventListener('pointerdown', function(e){ e.preventDefault(); send(b.dataset.go); });
  b.addEventListener('pointerup',     function(){ send('/stop'); });
  b.addEventListener('pointerleave',  function(){ send('/stop'); });
  b.addEventListener('pointercancel', function(){ send('/stop'); });
});

document.getElementById('stopBtn').addEventListener('click', function(){ send('/stop'); });

setInterval(function(){
  fetch('/count')
    .then(function(r){ return r.text(); })
    .then(function(t){ countEl.textContent = t.trim(); dot.classList.add('ok'); })
    .catch(function(){ dot.classList.remove('ok'); });
}, 500);
</script>
</body>
</html>
)rawliteral";