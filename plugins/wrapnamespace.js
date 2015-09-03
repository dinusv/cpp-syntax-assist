
function wrapNamespace(name){

    var cursorNode = codeBase.selectedNode();
    var fileNode   = cursorNode.parentFind('file');

    var astChildren = fileNode.astChildren();
    var len = astChildren.length;

    if ( len > 0 ){
        var first = astChildren[0];
        var last  = astChildren[len - 1];
        first.before('namespace ' + name + '{\n\n');
        last.afterln('\n} // namespace\n');
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}


NodeCollection.registerPlugin({
    'name' : 'wrapNamespace(name)',
    'usage' : 'wrapNamespace("name")',
    'description' : 'wraps the current class or namespace into the specified namespace "name".'
}).prototype.wrapNamespace = function(){
    this.nodes.forEach(function (v, i){
        wrapNamespace(v, false)
    });
    codeBase.save()
}
