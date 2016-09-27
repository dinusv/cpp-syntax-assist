
NodeCollection.describe({
    'name' : 'getResolution()',
    'usage' : 'getResolution()',
    'description' :
        'Get resolution breadcrumbs for first node as an array.'
}).extend({
    'getResolution' : function(properties){
        this.nodes.forEach(function (node, i){
            var breadCrumbs = [];
            var currentNode = classNode.astParent();
            while ( currentNode === null || currentNode.typeName() !== 'file' ){
                breadCrumbs.unshift(currentNode.identifier());
                currentNode = currentNode.astParent();
            }
            return breadCrumbs;
        });
    },
    'findClosestToResolution' : function(resolution){
        if ( this.nodes.length === 0 )
            return null
        var node = this.nodes[0]
        if ( node.typeName() !== 'file' )
            throw new Error("Node is not of file type: " + node.identifier())

        var resolutionString = ""
        for ( var i = 0; i < resolution.length; ++i ){
            var current = node.findFirst(resolution[i])
            if ( current ){
                node = current
            } else {
                for ( var j = i; j < resolution.length; ++j )
                    resolutionString += resolution[j] + '::'
                return { node: node, resolutionString: resolutionString }
            }
        }
        return { node: node, resolutionString: resolutionString }
    }
})
