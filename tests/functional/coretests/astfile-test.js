
CSATest.describe('astfile', function(test){

    test.scenario('get path data', function(){
        var file = codebase.parseFile('coretests/astfile-test.in')
        assert(file.fileName() === 'astfile-test.in')
        assert(file.fileNameWithoutExtension() === 'astfile-test')
        assert(file.extension() === 'in')
    })

    test.scenario('create location', function(){
        var file = codebase.parseFile('coretests/astfile-test.in')
        var location = file.createLocation(1, 1)
        assert(location.column() === 1)
        assert(location.line() === 1)

        var location2 = file.createLocation(0)
        assert(location2.column() === 1)
        assert(location2.line() === 1)
    })

    test.scenario('insert and read', function(){
        backup('coretests/astfile-test.in')
        var file = codebase.parseFile('coretests/astfile-test.in')
        var location = file.createLocation(2, 1)
        assert(file.insert('/*Comment*/', location))
        codebase.save()

        var readFrom = file.createLocation(2, 1)
        var readTo   = file.createLocation(2, 30)
        assert(file.read(readFrom, readTo) === '/*Comment*/class TestClass{};')
        restore()
    })

    test.scenario('erase and read', function(){
        backup('coretests/astfile-test.in')
        var file      = codebase.parseFile('coretests/astfile-test.in')
        var eraseFrom = file.createLocation(2, 7)
        var eraseTo   = file.createLocation(2, 11)
        assert(file.erase(eraseFrom, eraseTo))
        codebase.save()

        var readFrom = file.createLocation(2, 1)
        var readTo   = file.createLocation(2, 12)
        assert(file.read(readFrom, readTo) === 'class Class');
        restore()
    })

    test.scenario('insert, erase and read', function(){
        backup('coretests/astfile-test.in')
        var file      = codebase.parseFile('coretests/astfile-test.in')
        var eraseFrom = file.createLocation(2, 7)
        var eraseTo   = file.createLocation(2, 11)
        assert(file.erase(eraseFrom, eraseTo))
        assert(file.insert('T', file.createLocation(2, 7)))
        codebase.save()

        var readFrom = file.createLocation(2, 1)
        var readTo   = file.createLocation(2, 13)
        console.log(file.read(readFrom, readTo))
        assert(file.read(readFrom, readTo) === 'class TClass');
        restore()
    })
})
