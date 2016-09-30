CSATest.describe('ASTNode properties', function(test){

//    test.scenario('require file', function(){
//        backup('coretests/config-test.json')
//        var config = codebase.config().require('coretests/config-test.json')
//        assert(config.clangArgs.length === 2)
//        assert(config.clangArgs[0] === '-c')
//        assert(config.clangArgs[1] === '-x')

//        assert(config.headerFilePatterns.length === 1)
//        assert(config.headerFilePatterns[0] === '*config-test.in')

//        assert(config.sourceFilePatterns.length === 1)
//        assert(config.sourceFilePatterns[0] === '*test.cin')
//        restore()
//    })

//    test.scenario('save file', function(){
//        backup('coretests/config-test.json')
//        var config = codebase.config().require('coretests/config-test.json')
//        config.clangArgs = []
//        codebase.config().update(config)
//        codebase.config().save('coretests/config-test.json')

//        var updatedConfig = codebase.config().require('coretests/config-test.json')
//        assert(config.clangArgs.length === 0)

//        restore()
//    })

//    test.scenario('search files', function(){
//        codebase.config().update({
//            'headerFilePatterns' : ['*config-test.in']
//        })

//        codebase.parsePath('coretests')
//        assert(codebase.files().length === 1)
//        assert(codebase.files()[0].fileName() === 'config-test.in')
//    })

    test.scenario('include line', function(){
        var file = codebase.parseFile('coretests/config-test.in')
        //TODO: Use temporary include file to add line
//        assert(file !== null)
//        assert(file.children()[0].identifier() === 'EXPORT_MACRO')

//        codebase.config().includeLine('#define EXPORT_MACRO')
//        codebase.reparse()
        console.log(file.children()[0].identifier())
//        assert(file.children()[0].ide)
    })


})
