
CSATest.describe('invalid description', '')

CSATest.describe('invalid before scenario', function(test){
	
	test.beforeScenario('')
	
	test.scenario('this shouldn\'t get called', function(){
		throw new Error('This shouldn\'t get called')
	})
	
})

CSATest.describe('invalid after scenario', function(test){
	
	test.scenario('scenario', function(){
        assert(true)
	})

    test.afterScenario('')
	
})


CSATest.describe('invalid description', function(test){
	
	test.beforeScenario(function(){
		throw new Error('Failed to initialize test case for scenarios.')
	})
	
	test.afterScenario(function(){
		throw new Error('This shouldn\'t get called')
	})
	
	test.scenario('this shouldn\'t get called', function(){
		throw new Error('This shouldn\'t get called')
	})
	
	test.scenario('this shouldn\'t get called either', function(){
		throw new Error('This shouldn\'t get called')
	})
	
})
CSATest.describe('invalid description', function(test){
	
	test.beforeScenario(function(){
		throw new Error('Failed to initialize test case for scenarios.')
	})
	
	test.afterScenario(function(){
		throw new Error('This shouldn\'t get called')
	})
	
	test.scenario('this shouldn\'t get called', function(){
		throw new Error('This shouldn\'t get called')
	})
	
	test.scenario('this shouldn\'t get called either', function(){
		throw new Error('This shouldn\'t get called')
	})
	
})
