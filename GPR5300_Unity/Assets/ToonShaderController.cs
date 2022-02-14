using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ToonShaderController : MonoBehaviour
{

    Material mat = null;

    // Start is called before the first frame update
    void Start()
    {
        mat = GetComponent<Renderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        float pingPong = Mathf.PingPong(Time.time, 1.0f);

        Color col = Color.Lerp(Color.red, Color.green, pingPong);

        //mat.SetColor("_Tint", col);
        mat.SetFloat("_RimThreshold", pingPong);
    }
}
