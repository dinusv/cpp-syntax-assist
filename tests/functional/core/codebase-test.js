CSATest.describe('codebase parsing', function(test){
    test.scenario('simple parse', function(){
        var file = codebase.parseFile('core/codebase-test.in')
        assert(file !== null)
        assert(codebase.files().length === 1)
        assert(codebase.files()[0] === file)
        assert(file.children().length === 1)
        assert(file.firstChild().identifier() === 'ParserTest')
        assert(file.firstChild().typeName() === 'class')
    })

    test.scenario('simple reparse', function(){
        var file = codebase.parseFile('core/codebase-test.in')
        var reparsedFile = codebase.reparseFile('core/codebase-test.in')
        assert(reparsedFile !== 0)
        assert(codebase.files().length === 1)
        assert(file.children().length === 1)
    })
})

CSATest.describe('codebase locations', function(test){
    test.scenario('create location by offset', function(){
        var file     = codebase.parseFile('core/codebase-test.in')
        var location = codebase.createLocation(file.identifier(), 5)
        assert(location.offset() === 5)
        assert(location.line() === 1)
        assert(location.column() === 6)
        assert(location.filePath() === file.identifier())
    })
    test.scenario('create location by line and column', function(){
        var file     = codebase.parseFile('core/codebase-test.in')
        var location = codebase.createLocation(file.identifier(), 1, 6)
        assert(location.offset() === 5)
        assert(location.line() === 1)
        assert(location.column() === 6)
        assert(location.filePath() === file.identifier())
    })
    test.scenario('create invalid location', function(){
        var file     = codebase.parseFile('core/codebase-test.in')
        var location = codebase.createLocation(file.identifier(), file.size() - 1)
        assert(location.offset() === file.size() - 1)
        location = codebase.createLocation(file.identifier(), file.size())
        assert(location.offset() === 0)
    })
})

CSATest.describe('codebase file searching', function(test){
    test.scenario('find header for source', function(){
        //TODO
    })
    test.scenario('find source for header', function(){
        //TODO
    })
    test.scenario('find file', function(){

    })
    test.scenario('find file using search pattern', function(){

    })
    test.scenario('find data in file using search pattern', function(){

    })
})

CSATest.describe('codebase file creation and removal', function(test){
    test.scenario('create and remove file', function(){
        var file = codebase.createFile('core/codebase-test-create.in')
        file.insert('class CreatedClass{};', file.createLocation(0))
        codebase.save()
        file = codebase.files()[0]
        assert(file !== 0)
        assert(file.firstChild().identifier() === 'CreatedClass')
        assert(file.firstChild().typeName() === 'class')
        //TODO
//        codebase.removeFile(file)
//        assert(codebase.files().length === 0)
    })
    test.scenario('create, parse and remove file', function(){

    })
})


