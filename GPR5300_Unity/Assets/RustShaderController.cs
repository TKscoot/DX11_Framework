using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[ExecuteInEditMode]
public class RustShaderController : MonoBehaviour
{
    [SerializeField]
    Transform referencePlane = null;

    Material mat = null;

    Vector3 n = Vector3.zero;
    Vector3 p = Vector3.zero;

    float h = 0.0f;

    // Start is called before the first frame update
    void Start()
    {
        mat = GetComponent<Renderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        if (referencePlane.hasChanged)
        {
            n = transform.worldToLocalMatrix * referencePlane.up;
            p = referencePlane.position;

            h = Vector3.Dot(n, p);

            mat.SetVector("_N", n);
            mat.SetFloat("_H", h);
        }
    }
}
