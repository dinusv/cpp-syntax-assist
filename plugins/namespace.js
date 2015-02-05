
function namespace(name){

    var cursorNode = codeBase.cursorNode();
    var fileNode   = cursorNode.parentFind('file');

    var astChildren = fileNode.astChildren();
    var len = astChildren.length;
    if ( len > 0 ){
        var first = astChildren[0];
        var last  = astChildren[len - 1];
        first.before('namespace ' + name + '{\n\n');
        last.afterln('\n} // namespace\n');
    }

    codeBase.save();
}
