
NodeCollection.describe({
    'name' : 'wrapNamespace(name)',
    'usage' : 'wrapNamespace(\'name\')',
    'description' :
        'Wraps the current class or namespace into the specified namespace "name".'
}).extend({
    'wrapNamespace' : function(name){
        this.nodes.forEach(function (node, i){
            if ( node.typeName !== 'class' || node.typeName !== 'namespace')
                throw new Error("Node is not of namespace|class type.");

            node.before('namespace ' + name + '{\n\n');
            node.afterln('\n} // namespace\n');
        });
        codeBase.save()
    }
})
