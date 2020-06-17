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
    | SetStart
    | SetEnd;

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
        | SetStart => {
            let Some(start) = state.startCoord;
            state.grid[start.row][start.col] = setStart(state.grid[start.row][start.col]);
            {
                startCoord: None,
                endCoord: state.endCoord,
                grid: state.grid,
                foundPath: state.foundPath
            }            
        }
        | SetEnd => {
            let Some(end_) = state.endCoord;
            state.grid[end_.row][end_.row] = setEnd(state.grid[end_.row][end_.row]);
            {
                startCoord: state.startCoord,
                endCoord: None,
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
}

[@react.component]
let make = () => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);

    <div className="grid">
        <button 
            onClick=(_ => dispatch(SetStart))
            disabled=state.foundPath || coordNonExist(state.startCoord)
        >
            {React.string("Set Start")}
        </button> 
        <button
            onClick=(_ => dispatch(SetEnd))
            disabled=state.foundPath || coordNonExist(state.startCoord) || coordNonExist(state.endCoord)
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
                    action=Toggle({row: rowId, col: colId})
                    toggleUnit=dispatch
                    unit=state.grid[rowId][colId]
                    foundPath=state.foundPath                    
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