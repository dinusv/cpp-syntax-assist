CSATest.describe('restore', function(test){

    test.beforeScenario(function(){
        backup('internaltests/csa-restore-test.in')
    })

    test.scenario('write to file', function(){
        var file = codebase.parseFile('internaltests/csa-restore-test.in')
        assert(file !== null)
        file.insert('class ToRemove{};\n', file.createLocation(0))
        codebase.save()
    })

    test.scenario('delete file', function(){
        var file = codebase.parseFile('internaltests/csa-restore-test.in')
        assert(file !== null)
        assert(codebase.deleteFile(file))
    })

    test.afterScenario(function(){
        restore()
    })

})
