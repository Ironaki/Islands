[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () => {
  <main>
    <h2 className="title"> {React.string("Islands - A* Path Search")} </h2>
    <p className="intro">
      {React.string("Light Grey - Road - Cost 1 : : Dark Grey - Slope - Cost 4 : : Blue - Water - Impassable")}
    </p>
    <div className="side-bar">
      <p className="add-info">
        {React.string("Additional Info.")}
        <ul>
          <li>
            <a href="https://github.com/Ironaki/Islands#introduction" target="_blank"> {React.string("Usage")} </a>
          </li>
          <li>
            <a href="https://github.com/Ironaki/Islands#algorithm" target="_blank"> {React.string("Algorithm")} </a>
          </li>
          <li>
            <a href="https://github.com/Ironaki/Islands#inspiration" target="_blank">
              {React.string("Inspiration")}
            </a>
          </li>
        </ul>
      </p>
    </div>
    <World />
    <div className="footnote">
      <p> {React.string("Built by Zixuan (Armstrong) Li with ReasonML and ReasonReact. June 2020.")} </p>
      <a href="https://github.com/Ironaki" target="_blank"> {React.string("GitHub")} </a>
    </div>
  </main>;
};
