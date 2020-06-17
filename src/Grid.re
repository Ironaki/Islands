open SharedType;
open Unit;

type state = {
    startCoord: option(coord),
    endCoord: option(coord),    
    grid: array(array(unitType)),
    foundPath: bool
};

let initialState = {    
    startCoord: Some({row: 0, col:0}),
    endCoord: Some({row: 19, col: 19}),
    grid: {let base = Array.make_matrix(20, 20, Land(Road, Ordinary, NotPath))
            base[0][0] = Land(Road, Start, NotPath)
            base[19][19] = Land(Road, End, NotPath)
            base},
    foundPath: false
};

type action = 
    | Toggle(coord)
    | FindPath
    | EnableSetStart
    | EnableSetEnd
    | SetStart(coord)
    | SetEnd(coord);


let gridReducer = (state, action) => {
    switch (action) {
        | Toggle(coord) => {
            state.grid[coord.row][coord.col] = unitChange(state.grid[coord.row][coord.col]);
            {
                startCoord: state.startCoord,
                endCoord: state.endCoord,    
                grid: state.grid, 
                foundPath: false
            }
        }
        | FindPath => {
            let path = AStar.aStar(state.grid);
            let _ = List.map((coord) => state.grid[coord.row][coord.col] = unitToPath(state.grid[coord.row][coord.col]), path);
            {
                startCoord: state.startCoord,
                endCoord: state.endCoord,    
                grid: state.grid, 
                foundPath: !state.foundPath
            }
        }
        | EnableSetStart => {
            let Some(start) = state.startCoord;
            state.grid[start.row][start.col] = setOrdinary(state.grid[start.row][start.col]);
            {
                startCoord: None,
                endCoord: state.endCoord,
                grid: state.grid,
                foundPath: state.foundPath
            }            
        }
        | EnableSetEnd => {
            let Some(end_) = state.endCoord;
            state.grid[end_.row][end_.row] = setOrdinary(state.grid[end_.row][end_.row]);
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
                foundPath: state.foundPath
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
    }
};

let coordNonExist = (someCoord: option(coord)) => {
    switch (someCoord) {
        | None => true
        | Some(_) => false
    }
};



[@react.component]
let make = () => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);
    let noStart = coordNonExist(state.startCoord);
    let noEnd = coordNonExist(state.endCoord);

    <div className="grid">
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
        <button onClick=(_ => dispatch(FindPath))> 
            {
                switch (state.foundPath) {
                    | true => React.string("Clear Path")
                    | false => React.string("Find Path")
                }
            }
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
                key=("row "++string_of_int(rowId))
                name=("row "++string_of_int(rowId))
                >
                {row}
            </div>
        )
        |> React.array;}
    </div>
};