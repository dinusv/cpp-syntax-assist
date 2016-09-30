CSATest.describe('ASTNode properties', function(test){
    test.scenario('namespace type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var node = file.children()[1]
        assert(node !== null)
        assert(node.typeName() === 'namespace')
        assert(node.identifier() === 'testnamespace')
    })

    test.scenario('class type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var node = file.children()[1].children('class')[0]
        assert(node !== null)
        assert(node.typeName() === 'class')
        assert(node.identifier() === 'TestClass')
    })

    test.scenario('access specifier type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var accessNodes = file.children()[1].children('class')[0].children('access')
        assert(accessNodes.length === 2)
        assert(accessNodes[0].typeName() === 'access')
        assert(accessNodes[0].identifier() === 'public')
        assert(accessNodes[1].typeName() === 'access')
        assert(accessNodes[1].identifier() === 'private')
    })

    test.scenario('constructor type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
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
        assert(arguments[1].prop('type') === 'const std::string &')
        assert(arguments[2].identifier() === 'forwardType')
        assert(arguments[2].typeName() === 'argument')
        assert(arguments[2].prop('type') === 'ForwardDeclaredType *')
    })

    test.scenario('destructor type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var destructorNodes = file.find('*/', 'destructor')
        assert(destructorNodes.length === 1)
        assert(destructorNodes[0].identifier() === '~TestClass')
        assert(destructorNodes[0].typeName() === 'destructor')
        assert(destructorNodes[0].prop('virtual') === 'false')
    })

    test.scenario('field type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var fieldNodes = file.find('*/', 'field')
        assert(fieldNodes.length === 3)
        assert(fieldNodes[0].identifier() === 'intValue')
        assert(fieldNodes[0].typeName() === 'field')
        assert(fieldNodes[0].prop('type') === 'int')
        assert(fieldNodes[1].identifier() === 'stringType')
        assert(fieldNodes[1].typeName() === 'field')
        assert(fieldNodes[1].prop('type') === 'std::string')
        assert(fieldNodes[2].identifier() === 'm_forwardType')
        assert(fieldNodes[2].typeName() === 'field')
        assert(fieldNodes[2].prop('type') === 'ForwardDeclaredType *')
    })

    test.scenario('method type properties', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        assert(file !== null)
        var methods = file.find('*/', 'method')
        assert(methods.length === 5)
        assert(methods[0].identifier() === 'setValue')
        assert(methods[0].typeName() === 'method')
        assert(methods[0].prop('returnType') === 'void')
        assert(methods[0].prop('static') === false)
        assert(methods[0].prop('virtual') === true)
        assert(methods[0].prop('const') === false)
        assert(methods[0].prop('inline') === true)
        assert(methods[0].arguments().length === 1)
        assert(methods[0].arguments()[0].typeName() === 'argument')
        assert(methods[0].arguments()[0].prop('type') === 'int')
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
        assert(methods[1].arguments()[0].prop('type') === 'ForwardDeclaredType *')
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
        assert(methods[3].prop('returnType') === 'ForwardDeclaredType *')
        assert(methods[3].prop('static') === false)
        assert(methods[3].prop('virtual') === false)
        assert(methods[3].prop('const') === false)
        assert(methods[3].prop('inline') === true)
        assert(methods[3].arguments().length === 0)

        assert(methods[4].identifier() === 'getStringType')
        assert(methods[4].typeName() === 'method')
        assert(methods[4].prop('returnType') === 'const std::string &')
        assert(methods[4].prop('static') === false)
        assert(methods[4].prop('virtual') === false)
        assert(methods[4].prop('const') === true)
        assert(methods[4].prop('inline') === true)
        assert(methods[4].arguments().length === 0)
    })

})

CSATest.describe('ASTNode Traversal', function(test){

    test.scenario('neighbor based traversal', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var namespaceNode = file.firstChild('testnamespace')
        assert(namespaceNode !== null)
        assert(namespaceNode.identifier() === 'testnamespace')
        var namespaceNotFound = file.firstChild('testnamespace', 'class')
        assert(namespaceNotFound === null)

        assert(namespaceNode.children().length === 1)
        assert(namespaceNode.children()[0].typeName() === 'class')

        var classNode = namespaceNode.firstChild()
        assert(classNode.children('method').length === 5)
        assert(classNode.children('constructor').length === 2)
        assert(classNode.firstChild('', 'constructor').typeName() === 'constructor')

        assert(classNode.lastChild('', 'destructor').typeName() === 'destructor')
        assert(classNode.firstChild('getValue').typeName() === 'method')

        var getValueNode = classNode.firstChild('getValue')
        assert(getValueNode.prev().identifier() === 'setForwardType')
        assert(getValueNode.next().identifier() === 'getForwardType')
        assert(getValueNode.astParent().identifier() === 'TestClass')
    })

    test.scenario('search based traversal', function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var namespaceNode = file.firstChild('testnamespace')
        assert(namespaceNode !== null)

        var classNode = namespaceNode.firstChild()
        assert(namespaceNode.findNode(classNode) !== null)
        assert(namespaceNode.findNode(namespaceNode.astParent()) === null)

        assert(namespaceNode.find('#setValue/').length === 1)
        assert(namespaceNode.find('*void*Value*/')[0].identifier() === 'setValue')
        assert(namespaceNode.find('#setValue:numArgs[1]/')[0].identifier() === 'setValue')
        assert(namespaceNode.find('*(int)/')[0].identifier() === 'setValue')
        assert(namespaceNode.find('#*Value/').length === 3)
        assert(namespaceNode.find(':numArgs[3]/').length === 1)
        assert(namespaceNode.find(':numArgs[1]/').length === 2)
        assert(namespaceNode.find(':numArgs[0]/', 'method').length === 3)
        assert(namespaceNode.find('*/', 'destructor').length === 1)
        assert(namespaceNode.find('int *Value(*/').length === 1)
        assert(namespaceNode.find('*(int)/').length === 1)
        assert(namespaceNode.find('*(int,*string*)/')[0].typeName() === 'constructor')

        assert(namespaceNode.find('#intValue/')[0].parentFind('namespace').identifier() === 'testnamespace')
    })

})

CSATest.describe('ASTNode Breadcrumbs', function(test){

    test.scenario("constructor breadcrumbs", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var constructorNode = classNode.firstChild('', 'constructor')
        assert(constructorNode !== null)

        assert(file.breadcrumbs() + "/testnamespace/TestClass/TestClass()" === constructorNode.breadcrumbs())
    })

    test.scenario("destructor breadcrumbs", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var destructorNode = classNode.firstChild('', 'destructor')
        assert(destructorNode !== null)


        console.log(destructorNode.breadcrumbs())
        assert(file.breadcrumbs() + "/testnamespace/TestClass/~TestClass()" === destructorNode.breadcrumbs())
    })

    test.scenario("method breadcrumbs", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var setValueMethod = classNode.firstChild('setValue', 'method')
        assert(classNode.breadcrumbs() + "/virtual void setValue(int)" === setValueMethod.breadcrumbs())

        var setForwardTypeMethod = classNode.firstChild('setForwardType', 'method')
        assert(classNode.breadcrumbs() + "/void setForwardType(ForwardDeclaredType \\*)" === setForwardTypeMethod.breadcrumbs())

        var getValueMethod = classNode.firstChild('getValue', 'method')
        assert(classNode.breadcrumbs() + "/int getValue() const" === getValueMethod.breadcrumbs())

        var getForwardTypeMethod = classNode.firstChild('getForwardType', 'method')
        assert(classNode.breadcrumbs() + "/ForwardDeclaredType \\* getForwardType()" === getForwardTypeMethod.breadcrumbs())

        var getStringTypeMethod = classNode.firstChild('getStringType', 'method')
        assert(classNode.breadcrumbs() + "/const std\\:\\:string & getStringType() const" === getStringTypeMethod.breadcrumbs())
    })

    test.scenario("field breadcrumbs", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var intValueField = classNode.firstChild('intValue', 'field')
        assert(classNode.breadcrumbs() + "/int intValue" === intValueField.breadcrumbs())

        var stringTypeField = classNode.firstChild('stringType', 'field')
        assert(classNode.breadcrumbs() + "/std\\:\\:string stringType" === stringTypeField.breadcrumbs())

        var forwardTypeField = classNode.firstChild('m_forwardType', 'field')
        assert(classNode.breadcrumbs() + "/ForwardDeclaredType \\* m_forwardType" === forwardTypeField.breadcrumbs())
    })
})

CSATest.describe('ASTNode Modifiers', function(test){

    test.scenario("appending after a node", function(){
        backup('coretests/astnode-test.in')
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var intValueField = classNode.firstChild('intValue', 'field')
        intValueField.afterln('int insert;\n')
        codebase.save()

        classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)
        var insertedField = classNode.firstChild('insert', 'field')
        assert(insertedField !== null)

        restore()
    })

    test.scenario("prepending to a node", function(){
        backup('coretests/astnode-test.in')
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        var intValueField = classNode.firstChild('intValue', 'field')
        intValueField.before('int insert;\n')
        codebase.save()

        classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)
        var insertedField = classNode.firstChild('insert', 'field')
        assert(insertedField !== null)

        restore()
    })

})

CSATest.describe('ASTNode Locations', function(test){

    test.scenario("namespace locations", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var namespaceNode = file.firstChild('testnamespace')
        assert(namespaceNode !== null)

        assert(namespaceNode.rangeStartLocation().line() === 5)
        assert(namespaceNode.rangeStartLocation().column() === 1)

        assert(namespaceNode.rangeEndLocation().line() === 29)
        assert(namespaceNode.rangeEndLocation().column() === 2)

        assert(namespaceNode.bodyStartLocation().line() === 5)
        assert(namespaceNode.bodyStartLocation().column() === 25)

        assert(namespaceNode.bodyEndLocation().line() === 29)
        assert(namespaceNode.bodyEndLocation().column() === 1)
    })

    test.scenario("class locations", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var classNode = file.firstChild('testnamespace').firstChild()
        assert(classNode !== null)

        assert(classNode.rangeStartLocation().line() === 7)
        assert(classNode.rangeStartLocation().column() === 1)

        assert(classNode.rangeEndLocation().line() === 27)
        assert(classNode.rangeEndLocation().column() === 2)

        assert(classNode.bodyStartLocation().line() === 7)
        assert(classNode.bodyStartLocation().column() === 17)

        assert(classNode.bodyEndLocation().line() === 27)
        assert(classNode.bodyEndLocation().column() === 1)
    })

    test.scenario("method locations", function(){
        var file = codebase.parseFile('coretests/astnode-test.in')
        var methodNode = file.firstChild('testnamespace').firstChild().firstChild('getValue');
        assert(methodNode !== null)

        assert(methodNode.rangeStartLocation().line() === 16)
        assert(methodNode.rangeStartLocation().column() === 5)

        assert(methodNode.rangeEndLocation().line() === 16)
        assert(methodNode.rangeEndLocation().column() === 44)
    })

})
