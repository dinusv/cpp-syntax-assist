function insertAtLocation(line, column){
    var file     = codeBase.files()[0];
    var location = file.createLocation(line, column);
    var result   = file.insert("\n// Class InsertionTest\n", location);

    codeBase.save()

    return result;
}

debug.print("Loaded insertion.js")
