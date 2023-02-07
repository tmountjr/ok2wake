(()=>{var e={705:e=>{"use strict";e.exports=function(e){var t=[];return t.toString=function(){return this.map((function(t){var n="",a=void 0!==t[5];return t[4]&&(n+="@supports (".concat(t[4],") {")),t[2]&&(n+="@media ".concat(t[2]," {")),a&&(n+="@layer".concat(t[5].length>0?" ".concat(t[5]):""," {")),n+=e(t),a&&(n+="}"),t[2]&&(n+="}"),t[4]&&(n+="}"),n})).join("")},t.i=function(e,n,a,s,r){"string"==typeof e&&(e=[[null,e,void 0]]);var o={};if(a)for(var i=0;i<this.length;i++){var c=this[i][0];null!=c&&(o[c]=!0)}for(var l=0;l<e.length;l++){var u=[].concat(e[l]);a&&o[u[0]]||(void 0!==r&&(void 0===u[5]||(u[1]="@layer".concat(u[5].length>0?" ".concat(u[5]):""," {").concat(u[1],"}")),u[5]=r),n&&(u[2]?(u[1]="@media ".concat(u[2]," {").concat(u[1],"}"),u[2]=n):u[2]=n),s&&(u[4]?(u[1]="@supports (".concat(u[4],") {").concat(u[1],"}"),u[4]=s):u[4]="".concat(s)),t.push(u))}},t}},738:e=>{"use strict";e.exports=function(e){return e[1]}},21:(e,t)=>{"use strict";t.Z=(e,t)=>{const n=e.__vccOpts||e;for(const[e,a]of t)n[e]=a;return n}},499:e=>{e.exports={LEDEvent:class{static LED_STATE_WAKE=1;static LED_STATE_SLEEP=2;static LED_STATE_OFF=3;constructor(e,t,n){this.hour=e,this.minute=t,this.state=n}isFuture(e,t){return this.validateInput(e,t),this.hour>e||this.hour==e&&this.minute>=t}isCurrent(e,t){return this.validateInput(e,t),this.hour==e&&this.minute==t}isPast(e,t){return!(this.isFuture(e,t)||this.isCurrent(e,t))}validateInput(e,t){if(e<0||e>23||t<0||t>59)throw new Error("Hours must be between 0 and 23; minutes must be between 0 and 59.")}secondsSinceMidnight(){return 60*this.hour*60+60*this.minute}toJSON(){return{hour:this.hour,minute:this.minute,state:this.state}}}}},306:(e,t,n)=>{"use strict";n.d(t,{Z:()=>i});var a=n(738),s=n.n(a),r=n(705),o=n.n(r)()(s());o.push([e.id,".home-menu[data-v-10ffc6ae]{padding:.5em;box-shadow:0 1px 1px rgba(0,0,0,.1)}.header[data-v-10ffc6ae]{position:sticky;top:0;background:#fff}.header a[data-v-10ffc6ae]:hover,.header a[data-v-10ffc6ae]:focus{background:none;border:none}.content[data-v-10ffc6ae]{padding:1em}.buttonList[data-v-10ffc6ae]{display:flex;flex-wrap:wrap}.buttonList button[data-v-10ffc6ae]{flex-basis:100%;margin-bottom:10px;font-size:115%}fieldset.inline[data-v-10ffc6ae]{display:flex;align-items:flex-start}fieldset.inline span[data-v-10ffc6ae]{margin:0 .5em;align-self:center}button.pure-button.button-small[data-v-10ffc6ae]{font-size:75%}button.pure-button.button-error[data-v-10ffc6ae]{color:#fff;text-shadow:0 1px 1px rgba(0,0,0,.2);background:#ca3c3c}h2.condensed p.subtitle[data-v-10ffc6ae]{margin-top:0;font-size:60%;font-weight:initial;font-style:italic}",""]);const i=o},706:(e,t,n)=>{"use strict";n.d(t,{Z:()=>i});var a=n(738),s=n.n(a),r=n(705),o=n.n(r)()(s());o.push([e.id,"h2.condensed[data-v-54d5a9bc]{margin-top:0}",""]);const i=o},379:e=>{"use strict";var t=[];function n(e){for(var n=-1,a=0;a<t.length;a++)if(t[a].identifier===e){n=a;break}return n}function a(e,a){for(var r={},o=[],i=0;i<e.length;i++){var c=e[i],l=a.base?c[0]+a.base:c[0],u=r[l]||0,d="".concat(l," ").concat(u);r[l]=u+1;var p=n(d),m={css:c[1],media:c[2],sourceMap:c[3],supports:c[4],layer:c[5]};if(-1!==p)t[p].references++,t[p].updater(m);else{var v=s(m,a);a.byIndex=i,t.splice(i,0,{identifier:d,updater:v,references:1})}o.push(d)}return o}function s(e,t){var n=t.domAPI(t);return n.update(e),function(t){if(t){if(t.css===e.css&&t.media===e.media&&t.sourceMap===e.sourceMap&&t.supports===e.supports&&t.layer===e.layer)return;n.update(e=t)}else n.remove()}}e.exports=function(e,s){var r=a(e=e||[],s=s||{});return function(e){e=e||[];for(var o=0;o<r.length;o++){var i=n(r[o]);t[i].references--}for(var c=a(e,s),l=0;l<r.length;l++){var u=n(r[l]);0===t[u].references&&(t[u].updater(),t.splice(u,1))}r=c}}},569:e=>{"use strict";var t={};e.exports=function(e,n){var a=function(e){if(void 0===t[e]){var n=document.querySelector(e);if(window.HTMLIFrameElement&&n instanceof window.HTMLIFrameElement)try{n=n.contentDocument.head}catch(e){n=null}t[e]=n}return t[e]}(e);if(!a)throw new Error("Couldn't find a style target. This probably means that the value for the 'insert' parameter is invalid.");a.appendChild(n)}},216:e=>{"use strict";e.exports=function(e){var t=document.createElement("style");return e.setAttributes(t,e.attributes),e.insert(t,e.options),t}},565:(e,t,n)=>{"use strict";e.exports=function(e){var t=n.nc;t&&e.setAttribute("nonce",t)}},795:e=>{"use strict";e.exports=function(e){var t=e.insertStyleElement(e);return{update:function(n){!function(e,t,n){var a="";n.supports&&(a+="@supports (".concat(n.supports,") {")),n.media&&(a+="@media ".concat(n.media," {"));var s=void 0!==n.layer;s&&(a+="@layer".concat(n.layer.length>0?" ".concat(n.layer):""," {")),a+=n.css,s&&(a+="}"),n.media&&(a+="}"),n.supports&&(a+="}");var r=n.sourceMap;r&&"undefined"!=typeof btoa&&(a+="\n/*# sourceMappingURL=data:application/json;base64,".concat(btoa(unescape(encodeURIComponent(JSON.stringify(r))))," */")),t.styleTagTransform(a,e,t.options)}(t,e,n)},remove:function(){!function(e){if(null===e.parentNode)return!1;e.parentNode.removeChild(e)}(t)}}}},589:e=>{"use strict";e.exports=function(e,t){if(t.styleSheet)t.styleSheet.cssText=e;else{for(;t.firstChild;)t.removeChild(t.firstChild);t.appendChild(document.createTextNode(e))}}}},t={};function n(a){var s=t[a];if(void 0!==s)return s.exports;var r=t[a]={id:a,exports:{}};return e[a](r,r.exports,n),r.exports}n.n=e=>{var t=e&&e.__esModule?()=>e.default:()=>e;return n.d(t,{a:t}),t},n.d=(e,t)=>{for(var a in t)n.o(t,a)&&!n.o(e,a)&&Object.defineProperty(e,a,{enumerable:!0,get:t[a]})},n.o=(e,t)=>Object.prototype.hasOwnProperty.call(e,t),(()=>{"use strict";const e=Vue,t=t=>((0,e.pushScopeId)("data-v-10ffc6ae"),t=t(),(0,e.popScopeId)(),t),a={id:"control"},s=t((()=>(0,e.createElementVNode)("div",{class:"header"},[(0,e.createElementVNode)("div",{class:"home-menu pure-menu pure-menu-horizontal"},[(0,e.createElementVNode)("a",{href:"",class:"pure-menu-heading"},"ok2wake Control")])],-1))),r={class:"content"},o={class:"pure-g"},i={class:"pure-u-1"},c={class:"pure-g"},l={class:"pure-u-1"},u=t((()=>(0,e.createElementVNode)("h1",null,"Current Status",-1))),d={class:"pure-u-1"},p={class:"pure-form"},m={class:"inline"},v=["value","selected"],f=[t((()=>(0,e.createElementVNode)("i",{class:"fa-solid fa-save"},null,-1)))],h={class:"pure-u-1"},E=t((()=>(0,e.createElementVNode)("h1",null,"Update Status",-1))),b={class:"buttonList"},N={class:"pure-u-1"},V=t((()=>(0,e.createElementVNode)("h1",null,"Events",-1))),y={class:"pure-g"},g={key:0,class:"pure-u-1"},S=[t((()=>(0,e.createElementVNode)("p",null,'Click "Refresh Device Data" above to populate lists.',-1)))],w={class:"pure-u-1"},k=t((()=>(0,e.createElementVNode)("h2",{class:"condensed"},[(0,e.createTextVNode)(" Edit Schedule "),(0,e.createElementVNode)("p",{class:"subtitle"},"All times are relative to whatever the local timezone is set to.")],-1))),T={class:"pure-form"},x=["onClick"],D=[t((()=>(0,e.createElementVNode)("i",{class:"fa-solid fa-trash"},null,-1)))],C=["id","onUpdate:modelValue"],z=t((()=>(0,e.createElementVNode)("span",null,"h",-1))),B=["id","onUpdate:modelValue"],A=t((()=>(0,e.createElementVNode)("span",null,"m",-1))),L=["id","onUpdate:modelValue"],I=[t((()=>(0,e.createElementVNode)("option",{value:"1"},"Wake",-1))),t((()=>(0,e.createElementVNode)("option",{value:"2"},"Sleep",-1))),t((()=>(0,e.createElementVNode)("option",{value:"3"},"Off",-1)))],O={class:"pure-button-group",role:"group"},_=[t((()=>(0,e.createElementVNode)("i",{class:"fa-solid fa-plus"},null,-1)))],M=[t((()=>(0,e.createElementVNode)("i",{class:"fa-solid fa-save"},null,-1)))],Z=t=>((0,e.pushScopeId)("data-v-54d5a9bc"),t=t(),(0,e.popScopeId)(),t),F={class:"events"},U={class:"pure-u-1"},P=Z((()=>(0,e.createElementVNode)("h2",{class:"condensed"},"Current",-1))),j={class:"pure-u-1"},$=Z((()=>(0,e.createElementVNode)("h2",{class:"condensed"},"Upcoming",-1))),R={class:"event"};var W=n(499);const H={name:"event",components:[],props:{event:W.LEDEvent,current:Boolean},computed:{prefix(){return this.current?"Since":"At"},formattedTime(){return`${("00"+this.event.hour).slice(-2)}:${("00"+this.event.minute).slice(-2)}`},translatedStatus(){switch(this.event.state){case 1:return"Wake state";case 2:return"Sleep state";case 3:return"Off";default:return"N/A"}}}};var J=n(21);const q=(0,J.Z)(H,[["render",function(t,n,a,s,r,o){return(0,e.openBlock)(),(0,e.createElementBlock)("div",R,[(0,e.createElementVNode)("p",null,(0,e.toDisplayString)(o.prefix)+" "+(0,e.toDisplayString)(o.formattedTime)+" - "+(0,e.toDisplayString)(o.translatedStatus),1)])}]]),K={name:"events",components:[q],props:{events:Array},computed:{firstEvent(){return this.events[0]},upcomingEvents(){return this.events.slice(1)}}};var G=n(379),Q=n.n(G),X=n(795),Y=n.n(X),ee=n(569),te=n.n(ee),ne=n(565),ae=n.n(ne),se=n(216),re=n.n(se),oe=n(589),ie=n.n(oe),ce=n(706),le={};le.styleTagTransform=ie(),le.setAttributes=ae(),le.insert=te().bind(null,"head"),le.domAPI=Y(),le.insertStyleElement=re(),Q()(ce.Z,le),ce.Z&&ce.Z.locals&&ce.Z.locals;const ue=(0,J.Z)(K,[["render",function(t,n,a,s,r,o){const i=(0,e.resolveComponent)("Event");return(0,e.openBlock)(),(0,e.createElementBlock)("div",F,[(0,e.createElementVNode)("div",U,[P,(0,e.createVNode)(i,{event:o.firstEvent,current:!0},null,8,["event"])]),(0,e.createElementVNode)("div",j,[$,((0,e.openBlock)(!0),(0,e.createElementBlock)(e.Fragment,null,(0,e.renderList)(o.upcomingEvents,((t,n)=>((0,e.openBlock)(),(0,e.createBlock)(i,{key:`event-${n+1}`,event:t},null,8,["event"])))),128))])])}],["__scopeId","data-v-54d5a9bc"]]),de=async e=>{await fetch(`/status/set?state=${e}`,{method:"POST"})},pe=e=>{switch(e){case 1:return"Wake mode";case 2:return"Sleep mode";case 3:return"Off";default:return"N/A"}},me={name:"control",components:[ue],data:()=>({status:{},events:[],current:{},timezones:[],currentTimezone:""}),async mounted(){const e=await fetch("http://worldtimeapi.org/api/timezone"),t=await e.json();this.timezones=t,await this.update()},methods:{async update(){const e=await fetch("/status"),t=await e.json();this.status.t=t.t,this.status.s=t.s,this.status.o=t.o,this.status.tz=t.tz,this.currentTimezone=t.tz,await this.currentEvent()},async updateTz(e){e.preventDefault();const t=await fetch("/tz/set",{method:"POST",body:JSON.stringify({tz_name:this.currentTimezone})}),n=await t.json();this.status.t=n.t,this.status.s=n.s,this.status.o=n.o,this.status.tz=n.tz},async wake(){await de(1),await this.update()},async sleep(){await de(2),await this.update()},async off(){await de(3),await this.update()},async reset(){await fetch("/reset",{method:"POST"}),await this.update()},async currentEvent(){const e=await fetch("/events"),t=await e.json();let{events:n}=t;n=n.map((e=>new W.LEDEvent(e.hour,e.minute,e.state))),this.events=n},translatedStatus:e=>pe(e),removeEvent(e,t){e.preventDefault(),this.events=0===t?this.events.splice(1):[...this.events.slice(0,t),...this.events.slice(t+1)]},addEvent(e){e.preventDefault();const t=new Date;this.events.push(new W.LEDEvent(t.getHours(),t.getMinutes(),W.LEDEvent.LED_STATE_OFF))},async saveEvents(e){e.preventDefault(),this.events.sort(((e,t)=>e.secondsSinceMidnight()-t.secondsSinceMidnight())),await fetch("/events/set",{method:"POST",body:JSON.stringify(this.events)}),await this.update()}},computed:{deviceTime(){const{t:e=null,o:t=null,tz:n=null}=this.status;if(e&&t&&n){const a=1e3*(e-parseInt(t));return new Intl.DateTimeFormat("en-US",{dateStyle:"short",timeStyle:"medium",hour12:!1,timeZone:n}).format(a)}return"N/A"},deviceStatus(){return"s"in this.status?pe(this.status.s):"N/A"}}};var ve=n(306),fe={};fe.styleTagTransform=ie(),fe.setAttributes=ae(),fe.insert=te().bind(null,"head"),fe.domAPI=Y(),fe.insertStyleElement=re(),Q()(ve.Z,fe),ve.Z&&ve.Z.locals&&ve.Z.locals;const he=(0,J.Z)(me,[["render",function(t,n,Z,F,U,P){const j=(0,e.resolveComponent)("Events");return(0,e.openBlock)(),(0,e.createElementBlock)("div",a,[s,(0,e.createElementVNode)("div",r,[(0,e.createElementVNode)("div",o,[(0,e.createElementVNode)("div",i,[(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[0]||(n[0]=(...e)=>P.update&&P.update(...e))},"Refresh Device Data")])]),(0,e.createElementVNode)("div",c,[(0,e.createElementVNode)("div",l,[u,(0,e.createElementVNode)("p",null,"Current device time: "+(0,e.toDisplayString)(P.deviceTime),1),(0,e.createElementVNode)("p",null,"Current status: "+(0,e.toDisplayString)(P.deviceStatus),1),(0,e.createElementVNode)("p",null,"Current timezone: "+(0,e.toDisplayString)(U.status.tz),1)]),(0,e.createElementVNode)("div",d,[(0,e.createElementVNode)("form",p,[(0,e.createElementVNode)("fieldset",m,[(0,e.withDirectives)((0,e.createElementVNode)("select",{name:"tz",id:"tz",class:"pure-input-1","onUpdate:modelValue":n[1]||(n[1]=e=>U.currentTimezone=e)},[((0,e.openBlock)(!0),(0,e.createElementBlock)(e.Fragment,null,(0,e.renderList)(U.timezones,(t=>((0,e.openBlock)(),(0,e.createElementBlock)("option",{key:t,value:t,selected:t==U.status.tz},(0,e.toDisplayString)(t),9,v)))),128))],512),[[e.vModelSelect,U.currentTimezone]]),(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[2]||(n[2]=(...e)=>P.updateTz&&P.updateTz(...e))},f)])])]),(0,e.createElementVNode)("div",h,[E,(0,e.createElementVNode)("div",b,[(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[3]||(n[3]=(...e)=>P.wake&&P.wake(...e))},'Set to "Wake" mode'),(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[4]||(n[4]=(...e)=>P.sleep&&P.sleep(...e))},'Set to "Sleep" mode'),(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[5]||(n[5]=(...e)=>P.off&&P.off(...e))},'Set to "Off" mode'),(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[6]||(n[6]=(...e)=>P.reset&&P.reset(...e))},"Reset to default")])]),(0,e.createElementVNode)("div",N,[V,(0,e.createElementVNode)("div",y,[0==U.events.length?((0,e.openBlock)(),(0,e.createElementBlock)("div",g,S)):(0,e.createCommentVNode)("v-if",!0),U.events.length>0?((0,e.openBlock)(),(0,e.createBlock)(j,{key:1,events:U.events},null,8,["events"])):(0,e.createCommentVNode)("v-if",!0),(0,e.createElementVNode)("div",w,[k,(0,e.createElementVNode)("form",T,[((0,e.openBlock)(!0),(0,e.createElementBlock)(e.Fragment,null,(0,e.renderList)(U.events,((t,n)=>((0,e.openBlock)(),(0,e.createElementBlock)("fieldset",{class:"inline",key:`event-edit-${n}`},[(0,e.createElementVNode)("legend",null,[(0,e.createTextVNode)(" Event "+(0,e.toDisplayString)(n)+" ",1),(0,e.createElementVNode)("button",{class:"pure-button button-error button-small",onClick:e=>P.removeEvent(e,n)},D,8,x)]),(0,e.withDirectives)((0,e.createElementVNode)("input",{class:"pure-input-1-4",type:"number",name:"hour",id:`event-edit-${n}-hour`,"onUpdate:modelValue":e=>t.hour=e},null,8,C),[[e.vModelText,t.hour,void 0,{number:!0}]]),z,(0,e.withDirectives)((0,e.createElementVNode)("input",{class:"pure-input-1-4",type:"number",name:"minute",id:`event-edit-${n}-minute`,"onUpdate:modelValue":e=>t.minute=e},null,8,B),[[e.vModelText,t.minute,void 0,{number:!0}]]),A,(0,e.withDirectives)((0,e.createElementVNode)("select",{class:"pure-input-1-4",name:"state",id:`event-edit-${n}-state`,"onUpdate:modelValue":e=>t.state=e},I,8,L),[[e.vModelSelect,t.state,void 0,{number:!0}]])])))),128)),(0,e.createElementVNode)("div",O,[(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[7]||(n[7]=(...e)=>P.addEvent&&P.addEvent(...e))},_),(0,e.createElementVNode)("button",{class:"pure-button",onClick:n[8]||(n[8]=(...e)=>P.saveEvents&&P.saveEvents(...e))},M)])])])])])])])])}],["__scopeId","data-v-10ffc6ae"]]),Ee=(0,e.createApp)(he);Ee.component("Event",q),Ee.component("Events",ue),Ee.mount("#app")})()})();