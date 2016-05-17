CSATest.describe('ASTNode properties', function(test){
    test.scenario('namespace type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var node = file.firstChild()
        assert(node !== null)
        assert(node.typeName() === 'namespace')
        assert(node.identifier() === 'testnamespace')
    })

    test.scenario('class type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var node = file.firstChild().firstChild()
        assert(node !== null)
        assert(node.typeName() === 'class')
        assert(node.identifier() === 'TestClass')
    })


    test.scenario('access specifier type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var accessNodes = file.children('access')
        assert(accessNodes.length === 2)
        assert(accessNodes[0].typeName() === 'access')
        assert(accessNodes[0].identifier() === 'public')
        assert(accessNodes[1].typeName() === 'access')
        assert(accessNodes[1].identifier() === 'private')
    })

    test.scenario('constructor type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var constructorNodes = file.find('TestClass', 'constructor')
        assert(constructorNodes.length === 2)
        assert(constructorNodes[0].typeName() === 'constructor')
        assert(constructorNodes[0].identifier() === 'TestClass')
        assert(constructorNodes[0].arguments().length === 0)

        assert(constructorNodes[1].typeName() === 'constructor')
        assert(constructorNodes[1].identifier() === 'TestClass')
        var arguments = constructorNodes[1].arguments()
        assert(arguments.length === 3)
        assert(arguments[0].identifier() === 'value')
        assert(arguments[0].typeName() === 'argument')
        assert(arguments[0].prop('type') === 'int')
        assert(arguments[1].identifier() === 'stringType')
        assert(arguments[1].typeName() === 'argument')
        assert(arguments[1].prop('type') === 'const std::string&')
        assert(arguments[2].identifier() === 'forwardType')
        assert(arguments[2].typeName() === 'argument')
        assert(arguments[2].prop('type') === 'ForwardDeclaredType*')
    })

    test.scenario('destructor type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var destructorNodes = file.find('*', 'destructor')
        assert(destructorNodes.length === 1)
        assert(destructorNodes[0].identifier() === '~TestClass')
        assert(destructorNodex[0].typeName() === 'destructor')
    })

    test.scenario('field type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var fieldNodes = file.find('*', 'field')
        assert(fieldNodes.lenth === 3)
        assert(fieldNodes[0].identifier() === 'intValue')
        assert(fieldNodes[0].typeName() === 'field')
        assert(fieldNodes[0].prop('type') === 'int')
        assert(fieldNodes[1].identifier() === 'stringType')
        assert(fieldNodes[1].typeName() === 'field')
        assert(fieldNodes[1].prop('type') === 'std::string')
        assert(fieldNodes[2].identifier() === 'm_forwardType')
        assert(fieldNodes[2].typeName() === 'field')
        assert(fieldNodes[2].prop('type') === 'ForwardDeclaredType*')
    })


    test.scenario('method type properties', function(){
        var file = codebase.parseFile('core/astnode-test.in')
        assert(file !== null)
        var methods = file.find('*', 'method')
        assert(methods.lenght === 5)
        assert(methods[0].identifier() === 'setValue')
        assert(methods[0].typeName() === 'method')
        assert(methods[0].prop('returnType') === 'void')
        assert(methods[0].prop('static') === false)
        assert(methods[0].prop('virtual') === true)
        assert(methods[0].prop('const') === false)
        assert(methods[0].prop('inline') === true)
        assert(methods[0].arguments().length === 1)
        assert(methods[0].arguments()[0].typeName() === 'argument')
        assert(methods[0].argumnets()[0].prop('type') === 'int')
        assert(methods[0].arguments()[0].identifier() === 'value')

        assert(methods[1].identifier() === 'setForwardType')
        assert(methods[1].typeName() === 'method')
        assert(methods[1].prop('returnType') === 'void')
        assert(methods[1].prop('static') === false)
        assert(methods[1].prop('virtual') === false)
        assert(methods[1].prop('const') === false)
        assert(methods[1].prop('inline') === true)
        assert(methods[1].arguments().length === 1)
        assert(methods[1].arguments()[0].typeName() === 'argument')
        assert(methods[1].argumnets()[0].prop('type') === 'ForwardDeclaredType*')
        assert(methods[1].arguments()[0].identifier() === '')

        assert(methods[2].identifier() === 'getValue')
        assert(methods[2].typeName() === 'method')
        assert(methods[2].prop('returnType') === 'int')
        assert(methods[2].prop('static') === false)
        assert(methods[2].prop('virtual') === false)
        assert(methods[2].prop('const') === true)
        assert(methods[2].prop('inline') === true)
        assert(methods[2].arguments().length === 0)

        assert(methods[3].identifier() === 'getForwardType')
        assert(methods[3].typeName() === 'method')
        assert(methods[3].prop('returnType') === 'ForwardDeclaredType*')
        assert(methods[3].prop('static') === false)
        assert(methods[3].prop('virtual') === false)
        assert(methods[3].prop('const') === true)
        assert(methods[3].prop('inline') === true)
        assert(methods[3].arguments().length === 0)

        assert(methods[4].identifier() === 'getStringType')
        assert(methods[4].typeName() === 'method')
        assert(methods[4].prop('returnType') === 'const std::string&')
        assert(methods[3].prop('static') === false)
        assert(methods[3].prop('virtual') === false)
        assert(methods[3].prop('const') === true)
        assert(methods[3].prop('inline') === true)
        assert(methods[4].arguments().length === 0)
    })

})

CSATest.describe('ASTNode Traversal', function(test){



})

CSATest.describe('ASTNode Modifiers', function(test){



})

CSATest.describe('ASTNode Locations', function(test){


})

CSATest.describe('ASTNode Breadcrumbs', function(test){



})
