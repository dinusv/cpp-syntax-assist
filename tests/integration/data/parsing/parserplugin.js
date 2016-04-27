function parserplugin(){

    function recursiveParse(node){

        var parsedNode = {}
        parsedNode['nodeType'] = node.typeName()
        parsedNode['id']       = node.identifier()

        var nodeRangeStart = node.rangeStartLocation()
        var nodeRangeEnd   = node.rangeEndLocation()

        parsedNode['rangeStartOffset'] = nodeRangeStart.offset()
        parsedNode['rangeEndOffset']   = nodeRangeEnd.offset()

        var nodeChildren = node.children()
        if ( nodeChildren.length !== 0 ){
            parsedNode['children'] = [];
            for( var i = 0; i < nodeChildren.length; ++i )
                parsedNode['children'].push(recursiveParse(nodeChildren[i]))
        }

        if ( node.typeName() === 'file' ){
            parsedNode['id'] = node.rangeStartLocation().fileName()
        } else if ( node.typeName() === 'method' ){
            var nodeArguments = node.arguments();
            parsedNode['returnType'] = node.prop('returnType');
            parsedNode['arguments']  = [];
            if ( nodeArguments.length !== 0 ){
                for( var argIndex = 0; argIndex < nodeArguments.length; ++argIndex )
                    parsedNode['arguments'].push(recursiveParse(nodeArguments[argIndex]))
            }
        } else if ( node.typeName() === 'field' || node.typeName() === 'methodArgument' ){
            parsedNode['type'] = node.prop('type')
        }

        return parsedNode;
    }

    var file = codeBase.files()[0];
    return recursiveParse(file)
}
