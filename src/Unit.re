type terrain =
  | Road
  | Mountain;

type path =
  | InPath
  | NotPath;

type startEnd =
  | Ordinary
  | Start
  | End;

type unitType =
  | Water
  | Land(terrain, startEnd, path);

// Nice Emoji 🏁🚴🚵⛰🌊
// But too many of them give me trypophobia...
let randomDisplay = (emoji, toHash) =>
  if (Hashtbl.hash(toHash) mod 29 == 0) {
    React.string(emoji);
  } else {
    React.string({js| |js});
  };

let unitDisplay = (u: unitType, toHash) => {
  switch (u) {
  | Water => randomDisplay({js|🌊|js}, toHash)
  | Land(_, End, _) => React.string({js|🏁|js})
  | Land(Road, Start, _) => React.string({js|🚴|js})
  | Land(Mountain, Start, _) => React.string({js|🚵|js})
  | Land(Road, Ordinary, _) => React.string({js| |js})
  | Land(Mountain, Ordinary, InPath) => React.string({js|⛰|js})
  | Land(Mountain, Ordinary, NotPath) => randomDisplay({js|⛰|js}, toHash)
  };
};

// Get className, for CSS styling use
let unitClass = (u: unitType) => {
  switch (u) {
  | Water => "Water"
  | Land(_, _, InPath) => "InPath"
  | Land(Road, _, NotPath) => "Road"
  | Land(Mountain, _, NotPath) => "Mountain"
  };
};

/* Set unit to StartEnd point type */
let setPoint = (u: unitType, startEnd) => {
  switch (u) {
  | Land(Road, _, _) => Land(Road, startEnd, NotPath)
  | Land(Mountain, _, _) => Land(Mountain, startEnd, NotPath)
  | Water => Water
  };
};

let setStart = (u: unitType) => setPoint(u, Start);

let setEnd = (u: unitType) => setPoint(u, End);

let setOrdinary = (u: unitType) => setPoint(u, Ordinary);

/* Toggle unit when single click happens */
let unitToggle = (u: unitType) => {
  switch (u) {
  | Land(Road, Ordinary, NotPath) => Land(Mountain, Ordinary, NotPath)
  | Land(Mountain, Ordinary, NotPath) => Water
  | Water => Land(Road, Ordinary, NotPath)
  | _ => Land(Road, Ordinary, NotPath) // invalid input
  };
};

/* Get cost for A* algorithm use */
let unitCost = (u: unitType) => {
  switch (u) {
  | Land(Road, _, _) => 1
  | Land(Mountain, _, _) => 4
  | Water => max_int // invalid input
  };
};

/* Toggle path when click "Find Path"/"Clear Path" button */
let togglePath = (u: unitType) => {
  switch (u) {
  | Land(Road, Ordinary, NotPath) => Land(Road, Ordinary, InPath)
  | Land(Road, Start, NotPath) => Land(Road, Start, InPath)
  | Land(Road, End, NotPath) => Land(Road, End, InPath)
  | Land(Mountain, Ordinary, NotPath) => Land(Mountain, Ordinary, InPath)
  | Land(Mountain, Start, NotPath) => Land(Mountain, Start, InPath)
  | Land(Mountain, End, NotPath) => Land(Mountain, End, InPath)
  | Land(Road, Ordinary, InPath) => Land(Road, Ordinary, NotPath)
  | Land(Road, Start, InPath) => Land(Road, Start, NotPath)
  | Land(Road, End, InPath) => Land(Road, End, NotPath)
  | Land(Mountain, Ordinary, InPath) => Land(Mountain, Ordinary, NotPath)
  | Land(Mountain, Start, InPath) => Land(Mountain, Start, NotPath)
  | Land(Mountain, End, InPath) => Land(Mountain, End, NotPath)
  | Water => Water // invalid input
  };
};

let isWater = (u: unitType) => {
  switch (u) {
  | Water => true
  | Land(_, _, _) => false
  };
};

let isStartEnd = (u: unitType) => {
  switch (u) {
  | Land(_, Start, _) => true
  | Land(_, End, _) => true
  | _ => false
  };
};

/* For generate random grid */
let randomUnitInit = () => {
  let r = Random.int(10);
  if (r < 2) {
    Land(Mountain, Ordinary, NotPath);
  } else if (r >= 4) {
    Land(Road, Ordinary, NotPath);
  } else {
    Water;
  };
};

let randomStartEnd = startEnd => {
  let r = Random.int(5);
  if (r < 2) {
    Land(Mountain, startEnd, NotPath);
  } else {
    Land(Road, startEnd, NotPath);
  };
};

let randomStart = () => randomStartEnd(Start);

let randomEnd = () => randomStartEnd(End);

[@react.component]
let make = (~id, ~dispatch, ~unit as u, ~action, ~foundPath, ~startEndNotSet) => {
  <button
    id
    className={unitClass(u)}
    disabled={
      switch (foundPath, startEndNotSet, isWater(u), isStartEnd(u)) {
      | (true, _, _, _) => true
      | (false, false, _, true) => true
      | (false, false, _, false) => false
      | (false, true, true, _) => true
      | (false, true, false, false) => false
      | (false, true, false, true) => true
      }
    }
    onClick={_ => dispatch(action)}>
    {unitDisplay(u, id)}
  </button>;
};
