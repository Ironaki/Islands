open SharedType;
open Unit;

type state = {
    startCoord: option(coord),
    endCoord: option(coord),    
    grid: array(array(unitType)),
    foundPath: bool,
};

type action = 
    | Toggle(coord)
    | FindPath
    | EnableSetStart
    | EnableSetEnd
    | SetStart(coord)
    | SetEnd(coord)
    | Reconstruct(initType, int, int);


let initialState = {
    startCoord: Some(TokyoBay.tokyoBayStart),
    endCoord: Some(TokyoBay.tokyoBayEnd),
    grid: TokyoBay.tokyoBayGrid,
    foundPath: false
};

let gridReducer = (state, action) => {
    switch (action) {
        | Toggle(coord) => {
            state.grid[coord.row][coord.col] = unitChange(state.grid[coord.row][coord.col]);
            {...state, grid:state.grid}
        }
        | FindPath => {
            let Some(start_) = state.startCoord;
            let Some(end_) = state.endCoord;
            let path = AStar.aStar(state.grid, start_, end_);
            let _ = List.map((coord) => state.grid[coord.row][coord.col] = unitToPath(state.grid[coord.row][coord.col]), path);
            {
                startCoord: state.startCoord,
                endCoord: state.endCoord,    
                grid: state.grid, 
                foundPath: !state.foundPath,
            }
        }
        | EnableSetStart => {
            let Some(start) = state.startCoord;
            state.grid[start.row][start.col] = setOrdinary(state.grid[start.row][start.col]);
            {
                startCoord: None,
                endCoord: state.endCoord,
                grid: state.grid,
                foundPath: state.foundPath,
            }            
        }
        | EnableSetEnd => {
            let Some(end_) = state.endCoord;
            state.grid[end_.row][end_.col] = setOrdinary(state.grid[end_.row][end_.col]);
            {
                startCoord: state.startCoord,
                endCoord: None,
                grid: state.grid,
                foundPath: state.foundPath
            }                  
        }
        | SetStart(coord) => {
            state.grid[coord.row][coord.col] = setStart(state.grid[coord.row][coord.col]);
            {
                startCoord: Some(coord),
                endCoord: state.endCoord,
                grid: state.grid,
                foundPath: state.foundPath,
            }
        }
        | SetEnd(coord) => {
            state.grid[coord.row][coord.col] = setEnd(state.grid[coord.row][coord.col]);
            {
                startCoord: state.startCoord,
                endCoord: Some(coord),
                grid: state.grid,
                foundPath: state.foundPath
            }
        }
        | Reconstruct(init, rowSize, colSize) => {
            switch (init) {
                | Blank => {
                                startCoord: Some({row: 0, col: 0}),
                                endCoord: Some({row: rowSize-1, col: colSize-1}),
                                grid: {let base = Array.make_matrix(rowSize, colSize, Land(Mountain, Ordinary, NotPath))
                                        base[0][0] = Land(Road, Start, NotPath)
                                        base[rowSize-1][colSize-1] = Land(Road, End, NotPath)
                                        base},
                                foundPath: false
                            }
                | Random => { let base = Array.make_matrix(rowSize, colSize, Land(Road, Ordinary, NotPath));                            
                            Array.mapi((rowId, row) => 
                                Array.mapi((colId, _) => base[rowId][colId] = randomUnitInit(), row)
                            , base);
                            base[0][0] = randomStart();
                            base[rowSize-1][colSize-1] = randomEnd();
                            {
                                startCoord: Some({row: 0, col: 0}),
                                endCoord: Some({row: rowSize-1, col: colSize-1}),
                                grid: base,
                                foundPath: false
                            }
                            }
                | TokyoBay => initialState
            }
        }
    }
};

let coordNonExist = (someCoord: option(coord)) => {
    switch (someCoord) {
        | None => true
        | Some(_) => false
    }
};





[@react.component]
let make = (~rowSize, ~colSize, ~init, ~reconstructable) => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);
    let noStart = coordNonExist(state.startCoord);
    let noEnd = coordNonExist(state.endCoord);

    <div className="grid column">
        <button
            onClick=(_ => dispatch(Reconstruct(init, rowSize, colSize)))
            disabled=(!reconstructable)
        >  
        {React.string("Build New World")}
        </button>
        {state.grid
        |> Array.mapi((rowId, row) => 
            Array.mapi((colId, _) => 
                <Unit 
                    key=("coord "++string_of_int(rowId)++" "++string_of_int(colId))
                    id=("unit "++string_of_int(rowId)++" "++string_of_int(colId))
                    action=(switch (noStart, noEnd) {
                                | (true, _) => SetStart({row: rowId, col: colId})
                                | (_, true) => SetEnd({row: rowId, col: colId})
                                | _ => Toggle({row: rowId, col: colId})                                            
                            })
                    dispatch=dispatch
                    unit=state.grid[rowId][colId]
                    foundPath=state.foundPath
                    startEndNotSet=(coordNonExist(state.startCoord) || coordNonExist(state.endCoord))
                    >
                </Unit>
            , row) 
            |> React.array)
        |> Array.mapi((rowId, row) =>
            <div 
                className="grid row"
                key=("row "++string_of_int(rowId))
                name=("row "++string_of_int(rowId))
                >
                {row}
            </div>
        )
        |> React.array;}
        <button 
            onClick=(_ => dispatch(EnableSetStart))
            disabled=(state.foundPath || noStart || noEnd)
        >
            {React.string("Set Start")}
        </button> 
        <button
            onClick=(_ => dispatch(EnableSetEnd))
            disabled=(state.foundPath || noStart || noEnd)
        >
            {React.string("Set End")}
        </button>
        <button 
            onClick=(_ => dispatch(FindPath))
            disabled=(noStart || noEnd)
            >
                {
                    switch (state.foundPath) {
                        | true => React.string("Clear Path")
                        | false => React.string("Find Path")
                    }
                }
        </button>        
    </div>
};