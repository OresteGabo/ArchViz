using UnityEngine;
using System.Collections.Generic;
using Unity.XR.CoreUtils;

namespace ArchViz.Core
{
    [RequireComponent(typeof(XROrigin))]
    public class ArchVizManager : MonoBehaviour
    {
        public ArchVizSettings settings;
        private List<IArchVizModule> _modules = new List<IArchVizModule>();
        
        private HeightSystem _heightSystem;
        private NavigationSystem _navSystem;

        void Awake()
        {
            XROrigin origin = GetComponent<XROrigin>();
            
            // System Orchestration
            _heightSystem = new HeightSystem(origin, settings);
            _navSystem = new NavigationSystem(origin, settings);

            _modules.Add(_heightSystem);
            _modules.Add(_navSystem);

            foreach (var m in _modules) m.Initialize();
        }

        void Update()
        {
            foreach (var m in _modules) m.OnUpdate();

            // Quick Toggle for LinkedIn Demo
            if (Input.GetKeyDown(KeyCode.H)) _heightSystem.SetProfile(UserProfile.Adult);
            if (Input.GetKeyDown(KeyCode.E)) _heightSystem.SetProfile(UserProfile.Child);
        }
    }
}