CSATest.describe('failed before scenario', function(test){
	
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

CSATest.describe('failed after scenario', function(test){
	
	test.afterScenario(function(){
		throw new Error('Failed to clean test case for scenarios')
	})
	
	test.scenario('scenario 1', function(){
		assert(true)
	})
	
	test.scenario('scenario 2', function(){
		assert(true)
	})
	
})