using UnityEngine;
using Unity.XR.CoreUtils;

namespace ArchViz.Core
{
    public class HeightSystem : IArchVizModule
    {
        private XROrigin _origin;
        private ArchVizSettings _data;
        private float _targetY;

        public HeightSystem(XROrigin origin, ArchVizSettings data) 
        { 
            _origin = origin; 
            _data = data;
        }

        public void Initialize() => _targetY = _data.defaultHeight;

        public void OnUpdate()
        {
            if (_origin == null) return;
            _origin.CameraYOffset = Mathf.Lerp(_origin.CameraYOffset, _targetY, Time.deltaTime * _data.transitionSpeed);
        }

        public void SetProfile(UserProfile profile)
        {
            _targetY = profile == UserProfile.Adult ? _data.adultHeight : _data.childHeight;
        }

        public void Shutdown() { }
    }
}