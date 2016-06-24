
CSATest.describe('require', function(test){

    test.scenario('simple require', function(){
        assertThrows(function(){require('require-in')}, Error)

        var requireIn = require('require-in.js')
        assert(requireIn.requiredValue === 'value')
        assert(requireIn.requiredPlugin() === 'success')
        assert(requireIn.moduleRequiredPlugin() === 'module success')

        assert(nodes().requiredPlugin() === 'success')
    })

    test.scenario('require cache test', function(){
        var requireOnce = require('require-in.js')
        assert(NodeCollection.hasOwnProperty('toIncrement'));

        var requireCached = require('require-in.js')
        assert(NodeCollection.toIncrement === 0);
    })

    test.scenario('require from different paths', function(){
        var requireLocal = require('./require-in.js')

        assert(NodeCollection.hasOwnProperty('toIncrement'));
        var requireCached = require('require-in.js')
        assert(requireCached.requiredValue === 'value')
        assert(NodeCollection.toIncrement === 0)

        var requireAbsolute = require(__dirname + '/require-in.js')
        assert(requireCached.requiredValue === 'value')
        assert(NodeCollection.toIncrement === 0)
    })

    test.scenario('require single file loop', function(){
        assertThrows(function(){require('require-loop-in.js')}, Error)
    })

    test.scenario('require multi file loop', function(){
        assertThrows(function(){require('require-loop-multi-1-in.js')}, Error)
    })

})
