CSATest.describe('assertions', function(test){
	
	test.scenario('successful assertion', function(){
		assert(true)
	})
	
	test.scenario('failed assertion', function(){
		assert(false)
	})
	
	test.scenario('successful exception assertion', function(){
        assertThrows(function(){ throw new Error('Error'); })
	})
	
	test.scenario('failed exception assertion', function(){
        assertThrows(function(){ return 42; }, Error)
	})

    test.scenario('successful custom exception assertion', function(){
        function CustomError(){}
        assertThrows(function(){ throw new CustomError(); }, CustomError)
    })

    test.scenario('failed custom exception assertion', function(){
        function CustomError(){}
        assertThrows(function(){ throw new Error(); }, CustomError)
    })
	
})
