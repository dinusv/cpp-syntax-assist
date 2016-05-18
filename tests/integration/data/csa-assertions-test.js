CSATest.describe('assertions', function(test){
	
	test.scenario('successful assertion', function(){
		assert(true)
	})
	
	test.scenario('failed assertion', function(){
		assert(false)
	})
	
	test.scenario('successful exception assertion', function(){
		assertThrows(function(){ throw new Error('Error'); }, Error)
	})
	
	test.scenario('failed exception assertion', function(){
		assertThrows(function(){ return 42; }, Error);
	})
	
})