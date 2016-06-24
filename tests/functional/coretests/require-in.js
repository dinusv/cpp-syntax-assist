
exports.requiredPlugin = function(){ return 'success'; }
exports.requiredValue  = 'value'
module.exports.moduleRequiredPlugin = function(){ return 'module success'; }

NodeCollection.extend({
    requiredPlugin : function(){
        return 'success'
    }
})

if ( NodeCollection.hasOwnProperty('toIncrement') )
    NodeCollection.toIncrement++;
else
    NodeCollection.toIncrement = 0;
