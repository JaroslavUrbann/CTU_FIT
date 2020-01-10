/*
Creates an archive for an engine.
Empty archives don't exist.
*/
TARCHIVE * createArchive(TENGINE *engine){
    TARCHIVE *archive = (TARCHIVE *) malloc(sizeof(*archive));
    archive->m_Next = NULL;
    archive->m_Prev = NULL;
    archive->m_Engines = engine;
    return archive;
}


/*
Compares 2 engines based on the listBy attribute

+ if e1 > e2
0 if e1 == e2
- if e1 < e2
*/
int cmpEngines(TENGINE *engine1, TENGINE *engine2, int listBy){
    if (listBy == LIST_BY_YEAR)
        return (engine1->m_Year > engine2->m_Year) - (engine1->m_Year < engine2->m_Year);
    if (listBy == LIST_BY_TYPE)
        return strcmp(engine1->m_Type, engine2->m_Type);
    return 0;
}


/*
Inserts an archive in between 2 other archives
Returns the first archive
*/
TARCHIVE * insertArchive(TARCHIVE *arch1, TARCHIVE *arch2, TARCHIVE *newArch){
    newArch->m_Prev = arch1;
    newArch->m_Next = arch2;
    if (arch2)
        arch2->m_Prev = newArch;
    if (arch1)
        arch1->m_Next = newArch;
    return arch1;
}


/*
Inserts an engine in between 2 other engines
Returns the first engine
*/
TENGINE * insertEngine(TENGINE *engine1, TENGINE *engine2, TENGINE *newEngine){
    newEngine->m_Prev = engine1;
    newEngine->m_Next = engine2;
    if (engine2)
        engine2->m_Prev = newEngine;
    if (engine1)
        engine1->m_Next = newEngine;
    return engine1;
}


/*
Adds an engine into an archive that is ordered based on listBy
*/
TENGINE * AddEngineIntoArchive(TENGINE *head, TENGINE *engine, int listBy){
    // the current engine
    TENGINE *e = head;
    // previous engine
    TENGINE *e_Prev = NULL;
    while (1) {
        // inserts the engine if the next engine has higher year / type
        // or if the current engine is NULL
        if (!e || cmpEngines(engine, e, listBy) <= 0){
            // returns the original head of the linked list
            // or the engine, since it is the new head
            if (insertEngine(e_Prev, e, engine))
                return head;
            return engine;
        }
        e_Prev = e;
        e = e->m_Next;
    }
}


/*
Adds an engine to the archives
*/
TARCHIVE * AddEngine(TARCHIVE *archive, int listBy, TENGINE *engine){
    // this function expects these attributes to be NULL
    engine->m_Next = NULL;
    engine->m_Prev = NULL;

    TARCHIVE *a = archive;
    TARCHIVE *a_Prev = NULL;
    while (1) {
        // compares the current engine with the one being inserted
        // or becomes -1 if the current archive is NULL
        int cmp = a ? cmpEngines(engine, a->m_Engines, listBy) : -1;

        // the next archive has engines with a higher year / type
        // or the next archive is NULL
        if (cmp < 0){
            // creates a new archive for the engine
            TARCHIVE *newArch = createArchive(engine);
            // inserts it into the linked list of archives
            // returns the head of the archive
            // or newArch since it is the new head
            if (insertArchive(a_Prev, a, newArch))
                return archive;
            return newArch;
        }

        // the engine belongs to this archive
        if (cmp == 0){
            // adds the engine to the archive
            a->m_Engines = AddEngineIntoArchive(a->m_Engines, engine, !listBy);
            return archive;
        }

        a_Prev = a;
        a = a->m_Next;
    }
}


/*
Frees all allocated space
*/
void DelArchive(TARCHIVE *archive){
    // goes through all the archives
    TARCHIVE *a = archive;
    while (a){

        // goes through all the engines
        TENGINE *e = a->m_Engines;
        while (e){
            // frees each engine
            TENGINE *tmp_e = e;
            e = e->m_Next;
            free(tmp_e);
        }

        // frees each archive
        TARCHIVE *tmp_a = a;
        a = a->m_Next;
        free(tmp_a);
    }
}


/*
A recursive function that adds all engines of an archive to a new archive

archive - the new archive
listBy - parameter for the AddEngine function
engine - head of the old archive
*/
TARCHIVE * addEngines(TARCHIVE *archive, int listBy, TENGINE *engine){
    // goes to the end of the linked list first
    if (engine->m_Next)
        archive = addEngines(archive, listBy, engine->m_Next);

    // then adds all the engines and returns the new head of he archive
    return AddEngine(archive, listBy, engine);
}


/*
Reorders an archive based on the listBy parameter
*/
TARCHIVE * ReorderArchive(TARCHIVE *archive, int listBy){
    // creates a new archive
    TARCHIVE *newArchive = NULL;

    // goes through all the archives
    TARCHIVE *a = archive;
    while (a){

        // adds all engines of each archive to the new archives
        newArchive = addEngines(newArchive, listBy, a->m_Engines);

        // frees the old archives
        TARCHIVE *tmp_a = a;
        a = a->m_Next;
        free(tmp_a);
    }
    // returns the new reordered archives
    return newArchive;
}
