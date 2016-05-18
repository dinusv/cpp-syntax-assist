CSATest.describe('restore', function(test){

    test.beforeScenario(function(){
        CSATest.backup('core/csa-restore.in')
    })

    test.scenario('write to file', function(){
        var file = codebase.parseFile('core/csa-restore.in')
        file.insert('class ToRemove{};\n', file.createLocation(0))
        codebase.save()
    })

    test.scenario('remove file', function(){
        var file = codebase.parseFile('core/csa-restore.in')
        assert(codebase.removeFile(file))
    })

    test.afterScenario(function(){
        CSATest.restore('core/csa-restore.in')
    })

})
