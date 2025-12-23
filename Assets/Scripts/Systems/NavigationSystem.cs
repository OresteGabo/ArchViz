using UnityEngine;
using Unity.XR.CoreUtils;

namespace ArchViz.Core
{
    public class NavigationSystem : IArchVizModule
    {
        private XROrigin _origin;
        private ArchVizSettings _data;

        public NavigationSystem(XROrigin origin, ArchVizSettings data)
        {
            _origin = origin;
            _data = data;
        }

        public void Initialize() => Debug.Log("Navigation System Online.");

        public void OnUpdate()
        {
            // Implementation for 1:1 real world sync movement logic
        }

        public void Shutdown() { }
    }
}