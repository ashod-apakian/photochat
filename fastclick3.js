(function( window ) {
 var Tap={};
 var utils={};

 utils.attachEvent=function(element,eventName,callback) { if ('addEventListener' in window) {            return element.addEventListener(eventName,callback,false);        }    };
 utils.fireFakeEvent=function(e,eventName) {    if (document.createEvent)    {         return e.target.dispatchEvent(utils.createEvent(e,eventName));         }    };
 utils.createEvent=function(olde,name) {
        if (document.createEvent)
        {
            var evnt=window.document.createEvent('HTMLEvents');
            evnt.initEvent(name,true,true);
            evnt.olde=olde;
            evnt.eventName=name;
            return evnt;
        }
       else  {       Oof();        }
    };

 utils.getRealEvent=function(e) {   return e;     };

 var eventMatrix=[{
         test: ('propertyIsEnumerable' in window||'hasOwnProperty' in document)&&(window.propertyIsEnumerable('ontouchstart')||document.hasOwnProperty('ontouchstart')||window.hasOwnProperty('ontouchstart')),
        events: {            start: 'touchstart',           move: 'touchmove',           end: 'touchend'        }
    },{
        test: window.navigator.msPointerEnabled,  events: {  start: 'MSPointerDown',move: 'MSPointerMove',      end: 'MSPointerUp'        }
    },{
         test: window.navigator.pointerEnabled,   events: {            start: 'pointerdown',           move: 'pointermove',           end: 'pointerup'        }
    }];

    Tap.options={        eventName: 'tap',       fingerMaxOffset: 22   };

 var attachDeviceEvent,init,handlers,deviceEvents,coords={};
 attachDeviceEvent=function(eventName) {        return utils.attachEvent(document.documentElement,deviceEvents[eventName],handlers[eventName]);    };

    handlers={
        start: function(e)
        {
            e=utils.getRealEvent(e);
            coords.start=[e.pageX,e.pageY];
            coords.offset=[0,0];
           if(!utils.fireFakeEvent(e,Tap.options.eventName))
           {
                if (window.navigator.msPointerEnabled||window.navigator.pointerEnabled)
                {
                    var preventDefault=function(clickEvent)  {  clickEvent.preventDefault();    e.target.removeEventListener('click',preventDefault);                    };
                    e.target.addEventListener('click',preventDefault,false);
                }
                e.preventDefault();
            }
        },

        move: function(e)
        {
            if (!coords.start&&!coords.move) {                return false;            }
            e=utils.getRealEvent(e);
            coords.move=[e.pageX,e.pageY];
            coords.offset=[     Math.abs(coords.move[0] - coords.start[0]),               Math.abs(coords.move[1] - coords.start[1])            ];
           if(!utils.fireFakeEvent(e,Tap.options.eventName))
           {
                if (window.navigator.msPointerEnabled||window.navigator.pointerEnabled)
                {
                    var preventDefault=function(clickEvent)     {    clickEvent.preventDefault();    e.target.removeEventListener('click',preventDefault);                    };
                    e.target.addEventListener('click',preventDefault,false);
                }
                e.preventDefault();
            }
        },

        end: function(e)
        {
          e=utils.getRealEvent(e);
            if(!utils.fireFakeEvent(e,Tap.options.eventName))
           {
                if (window.navigator.msPointerEnabled||window.navigator.pointerEnabled)
                {
                    var preventDefault=function(clickEvent)    {    clickEvent.preventDefault();      e.target.removeEventListener('click',preventDefault);                 };
                    e.target.addEventListener('click',preventDefault,false);
                }
                e.preventDefault();
            }
            coords={};
        },

        click: function(e)
        {
            if (!utils.fireFakeEvent(e,Tap.options.eventName)) {                return e.preventDefault();            }
        }
    };

    init=function() {
        var i=0;
        for (; i < eventMatrix.length; i++) {
            if (eventMatrix[i].test) {
                deviceEvents=eventMatrix[i].events;
                attachDeviceEvent('start');
                attachDeviceEvent('move');
                attachDeviceEvent('end');
                break;
            }
        }
        return utils.attachEvent(document.documentElement,'click',handlers.click);
    };
    utils.attachEvent(window,'load',init);
 if(typeof define==='function'&&define.amd) { define(function() {            init();            return Tap;        });   }
 else                                       { window.Tap=Tap;   }

})( window );
