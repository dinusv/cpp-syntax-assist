
CSATest.describe('invalid before scenario', function(test){
	
	test.beforeScenario('')
	
    test.scenario('this shouldn\'t get called', function(){
        assert(true)
	})
	
})

CSATest.describe('invalid after scenario', function(test){

	test.scenario('scenario', function(){
        assert(true)
	})

    test.afterScenario('')
	
})

CSATest.describe('invalid description', function(test){
	
    test.scenario('uncallable', '')

})
