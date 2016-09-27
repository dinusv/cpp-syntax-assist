
//TODO
//function addConstructor(properties, initFields, node, save){

//    var isInline = false;
//    if ( typeof properties !== 'undefinde' )
//        isInline = properties === 'i';

//    function findNode(){
//        if (typeof node === 'undefined'){
//            var cursorNode = codeBase.selectedNode();
//            if ( cursorNode.typeName() === 'class' )
//                return cursorNode;
//            else
//                return cursorNode.parentFind('class');
//        } else {
//            return (node.typeName() === 'class' ? node : null);
//        }
//    }

//    function declareConstructor(classNode, paramList){
//        var constructorDeclaration = classNode.identifier() + '(' + paramList + ');\n';

//        var publicAccess = classNode.firstChild('public', 'access');
//        if ( publicAccess !== null ){
//            publicAccess.after('\n    ' + constructorDeclaration + '\n');
//        } else {
//            classNode.append('\npublic:\n    ' + constructorDeclaration);
//        }
//    }

//    function extractParamName(fieldDeclaration){
//        if ( fieldDeclaration.indexOf('_') === 0 )
//            return fieldDeclaration.slice(1);
//        else if ( fieldDeclaration.indexOf('m_') === 0 )
//            return fieldDeclaration.slice(2);
//        else if ( fieldDeclaration.indexOf('_') === fieldDeclaration.length - 1)
//            return fieldDeclaration.slice(0, fieldDeclaration.length - 1);
//        return fieldDeclaration + 'P';
//    }

//    function buildConstructorParamList(fields){
//        var nodes = typeof fields === 'undefined' ? [] : fields instanceof NodeCollection ? fields.nodes : fields
//        var paramList = ''
//        for ( var i = 0; i < nodes.length; ++i ){
//            var currentNode = nodes[i]
//            if ( paramList !== '' )
//                paramList += ', '
//            paramList += 'const ' + currentNode.prop("type") + '& ' + extractParamName(currentNode.identifier())
//        }
//        return paramList
//    }

//    function buildConstructorInitializers(fields){
//        var nodes = typeof fields === 'undefined' ? [] : fields instanceof NodeCollection ? fields.nodes : fields
//        var initializers = ''
//        for( var i = 0; i < nodes.length; ++i ){
//            var currentNode = nodes[i]
//            initializers += initializers === '' ? '\n    : ' : '\n    , '
//            initializers += currentNode.identifier() + '(' + extractParamName(currentNode.identifier()) + ')'
//        }
//        return initializers
//    }

//    function buildConstructorDefinition(name, paramList, initializers){
//        return name + '(' + paramList + ')' + initializers + '\n{\n}\n'
//    }

//    function getSourceNamespaceAndPosition(classNode){
//        var breadCrumbs = '';
//        var currentNode = classNode.astParent();
//        while ( currentNode.typeName() !== 'file' || currentNode === null ){
//            breadCrumbs = currentNode.identifier() + '/' + breadCrumbs;
//            currentNode = currentNode.astParent();
//        }
//        if ( currentNode === null )
//            return null;

//        var sourceFileNode = codeBase.findSource(currentNode.identifier());
//        if ( sourceFileNode === null )
//            return null;

//        var usedNode = sourceFileNode;
//        var cppBreadCrumbs = '';
//        while ( breadCrumbs !== '' ){
//            var foundNode = sourceFileNode.findFirst(breadCrumbs);
//            if ( foundNode === null ){
//                var slashBackIndex = breadCrumbs.lastIndexOf('/', breadCrumbs.length - 2);

//                if ( slashBackIndex > 0 ){
//                    cppBreadCrumbs =
//                        cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
//                    breadCrumbs    = breadCrumbs.slice(0, slashBackIndex + 1);
//                } else {
//                    cppBreadCrumbs =
//                        cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
//                    breadCrumbs    = '';
//                }

//            } else {
//                usedNode = foundNode;
//                break;
//            }
//        }
//        return { node: usedNode, cppBreadCrumbs: cppBreadCrumbs };
//    }

//    var classNode = findNode();
//    if ( classNode === null )
//        throw new Error("Cannot find specified class.");

//    // Add constructor declaration

//    var parameterList   = buildConstructorParamList(initFields)
//    var initializerList = buildConstructorInitializers(initFields)

//    declareConstructor(classNode, parameterList)

//    // Add constructor definition

//    if ( isInline ){
//        classNode.afterln('\ninline ' + classNode.identifier() + '::' +
//                          buildConstructorDefinition(classNode.identifier(), parameterList, initializerList))
//    } else {
//        var namespaceAndPosition = getSourceNamespaceAndPosition(classNode);
//        if ( namespaceAndPosition === null ){
//            classNode.afterln('\ninline ' + classNode.identifier() + '::' +
//                              buildConstructorDefinition(classNode.identifier(), parameterList, initializerList));
//        } else {
//            namespaceAndPosition.node.append(
//                '\n' + namespaceAndPosition.cppBreadCrumbs + classNode.identifier() + '::' +
//                buildConstructorDefinition(classNode.identifier(), parameterList, initializerList)
//            );
//        }
//    }

//    if (typeof save !== 'undefined' ? save : true)
//        codeBase.save()
//}

//NodeCollection.registerPlugin({
//    'name' : 'addConstructor(properties, initFields)',
//    'usage' : 'addConstructor()',
//    'description' :
//        'Adds a constructor to the current class or parent class.\n' +
//        'Params:\n' +
//            '\tproperties <String> : \'i\' - to enable inline constructor.\n' +
//            '\tinitFields <Array> : Array or NodeCollection of node fields to initialize.\n'
//}).prototype.addConstructor = function(properties, initFields){
//    this.nodes.forEach(function (v, i){
//        addConstructor(properties, initFields, v, false)
//    });
//    codeBase.save()
//}
