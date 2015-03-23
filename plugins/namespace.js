
function namespace(name){

    var cursorNode = codeBase.cursorNode();
    var fileNode   = cursorNode.parentFind('file');

    var astChildren = fileNode.astChildren();
    var len = astChildren.length;

    print(astChildren.length);

    var tokenList = astChildren[0].associatedTokens();
    print(tokenList.length);
    for ( var i in tokenList){
        var token = tokenList[i];
        print(token.name() + ' ' + token.kind() + '\n');

//        print('\n');
////        print(token.name());
//        print(token.kind());
    }


    var tt = new Token();
    print('---------------------+++');
    for ( var i in Token ){
        //if ( i === 'TokenKind' || i === 'Punctuation')
            print(i + ' ' + Token[i]);
    }
    print(Token.Punctuation)
    print(Token.Keyword)
    print(Token.Identifier)
    print(Token.Literal)
    print(Token.Comment)

//    if ( len > 0 ){
//        var first = astChildren[0];
//        var last  = astChildren[len - 1];
//        first.before('namespace ' + name + '{\n\n');
//        last.afterln('\n} // namespace\n');
//    }

//    codeBase.save();
}
