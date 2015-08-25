

function addConstructor(properties, initFields, node, save){

    var isInline = false;
    if ( typeof properties !== 'undefinde' )
        isInline = properties === 'i';

    function findNode(){
        if (typeof node === 'undefined'){
            var cursorNode = codeBase.cursorNode();
            if ( cursorNode.typeName() === 'class' )
                return cursorNode;
            else
                return cursorNode.parentFind('class');
        } else {
            return (node.typeName() === 'class' ? node : null);
        }
    }

    function declareConstructor(classNode, paramList){
        var destructorDeclaration = classNode.identifier() + '(' + paramList + ');\n';
        var constructorArray = classNode.children('constructor');
        if ( constructorArray.length > 0 ){
            constructorArray[constructorArray.length - 1].afterln(destructorDeclaration + '\n');
        } else {
            var publicAccess = classNode.firstChild('public', 'access');
            if ( publicAccess !== null ){
                publicAccess.after('\n' + destructorDeclaration + '\n');
            } else {
                classNode.append('\npublic:\n' + destructorDeclaration);
            }
        }
    }

    function buildConstructorParamList(params){
        //TODO: Implement
        return ''
    }

    function buildConstructorInitializers(fields){
        //TODO: Implement
        return ''
    }

    function getSourceNamespaceAndPosition(classNode){
        var breadCrumbs = '';
        var currentNode = classNode.astParent();
        while ( currentNode.typeName() !== 'file' || currentNode === null ){
            breadCrumbs = currentNode.identifier() + '/' + breadCrumbs;
            currentNode = currentNode.astParent();
        }
        if ( currentNode === null )
            return null;

        var sourceFileNode = codeBase.findSource(currentNode.identifier());
        if ( sourceFileNode === null )
            return null;

        var usedNode = sourceFileNode;
        var cppBreadCrumbs = '';
        while ( breadCrumbs !== '' ){
            var foundNode = sourceFileNode.findFirst(breadCrumbs);
            if ( foundNode === null ){
                var slashBackIndex = breadCrumbs.lastIndexOf('/', breadCrumbs.length - 2);

                if ( slashBackIndex > 0 ){
                    cppBreadCrumbs =
                        cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
                    breadCrumbs    = breadCrumbs.slice(0, slashBackIndex + 1);
                } else {
                    cppBreadCrumbs =
                        cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
                    breadCrumbs    = '';
                }

            } else {
                usedNode = foundNode;
                break;
            }
        }
        return { node: usedNode, cppBreadCrumbs: cppBreadCrumbs };
    }

    var classNode = findNode();
    if ( classNode === null )
        throw new Error("Cannot find specified class.");

    // Add destructor declaration

    var parameterList = buildConstructorParamList(initFields);

    declareConstructor(classNode, parameterList)

    // Add destructor definition

    if ( isInline ){
        classNode.afterln('\ninline ' + classNode.identifier() + '::' + classNode.identifier() + '(){\n}\n');
    } else {
        var namespaceAndPosition = getSourceNamespaceAndPosition(classNode);
        if ( namespaceAndPosition === null ){
            classNode.afterln('\ninline ' + classNode.identifier() + '::' + classNode.identifier() + '(){\n}\n');
        } else {
            namespaceAndPosition.node.append(
                '\n' + namespaceAndPosition.cppBreadCrumbs + classNode.identifier() + '::' +
                classNode.identifier() + '(){\n}\n'
            );
        }
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addConstructor',
    'usage' : 'addConstructor()',
    'description' :
        'Adds a constructor to the current class or parent class.' +
        'Params:\n' +
            '\tproperties <bool> : \'i\' - to enable inline constructor.' +
            '\tinitFields <Array> : array of node fields to initialize.'

}).prototype.addConstructor = function(properties, initFields){
    this.nodes.forEach(function (v, i){
        addConstructor(properties, initFields, v, false)
    });
    codeBase.save()
}
