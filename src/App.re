[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () => {
  <main> <h2 className="title"> {React.string("Islands - A* Path Search")} </h2> <World /> </main>;
};
