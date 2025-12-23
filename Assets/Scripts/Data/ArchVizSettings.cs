using UnityEngine;

namespace ArchViz.Core
{
    [CreateAssetMenu(fileName = "NewArchVizSettings", menuName = "ArchViz/Settings")]
    public class ArchVizSettings : ScriptableObject
    {
        [Header("Height Profiles (Meters)")]
        public float adultHeight = 1.80f;
        public float childHeight = 1.10f;
        public float defaultHeight = 1.70f;
        public float transitionSpeed = 5f;

        [Header("Movement Settings")]
        public float stepMultiplier = 1.0f;
        public bool useRealWorldSync = true;
    }
}