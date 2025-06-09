using UnityEngine;

public class RayTracingMaster : MonoBehaviour
{
    public ComputeShader RayTracingShader;
    private RenderTexture targetRenderTxture;
    private Camera mainCam;

    public Texture skyboxTexture;

    // Count samples and make sure of image effect shader.
    // Trace not one but multiple rays per pixel, where
    // each ray gets a random offset inside the pixel's region. 
    // To maintain fps, doing progressive sample, meaning that we will trace one pixel each frame
    // and average the result over time if the camera didnt move.
    // Every time the camera moves, or any other param like scene gemoetry, we need start all over
    private uint _currentSample = 0;
    private Material addMaterial;

    void Awake()
    {
        mainCam = GetComponent<Camera>();
    }

    // Automatically called by Unity whenever camera has finished rendering!
    // Must be attached to camera?
    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        SetShaderParameters();
        Render(destination);
    }

    private void Render(RenderTexture destination)
    {
        // Make sure we have a current render target
        InitRenderTexture();

        // Set the target and dispatch compute shader
        RayTracingShader.SetTexture(0, "Result", targetRenderTxture);

        // We want to spawn one thread per pixel of the render target.
        // Default thread group is 8,8,1 so we'll spawn one thread group per
        // 8x8 pixels
        int threadGroupsX = Mathf.CeilToInt(Screen.width / 8.0f);
        int threadGroupsY = Mathf.CeilToInt(Screen.height / 8.0f);
        RayTracingShader.Dispatch(0, threadGroupsX, threadGroupsY, 1);

        // Blit the result texture to the screen.
        // Means to write our result to screen.

        if(addMaterial == null)
        {
            addMaterial = new Material(Shader.Find("Hidden/AddShader2"));
        }

        addMaterial.SetFloat("_Sample", _currentSample);
        Graphics.Blit(targetRenderTxture, destination, addMaterial);
        _currentSample ++;
    }

    private void InitRenderTexture()
    {
        if(targetRenderTxture == null || targetRenderTxture.width != Screen.width || targetRenderTxture.height != Screen.height)
        {
            // Render texture if we already have one. 
            // Release hardware resources used by render texture
            if(targetRenderTxture != null)
            {
                targetRenderTxture.Release();
            }

            // Get render target for ray tracing
            targetRenderTxture = new RenderTexture(Screen.width, Screen.height, 0,
            RenderTextureFormat.ARGBFloat, RenderTextureReadWrite.Linear);
            targetRenderTxture.enableRandomWrite = true;
            targetRenderTxture.Create();
        }
    }

    void SetShaderParameters()
    {
        RayTracingShader.SetMatrix("_CameraToWorld", mainCam.cameraToWorldMatrix);
        RayTracingShader.SetMatrix("_CameraInverseProjection", mainCam.projectionMatrix.inverse);
        RayTracingShader.SetTexture(0, "_SkyboxTexture", skyboxTexture);

        // each ray gets a random offset inside the pixels region for anti-aliasing
        RayTracingShader.SetVector("_PixelOffset", new Vector2(UnityEngine.Random.value, UnityEngine.Random.value));

    }
    void Update()
    {
        if(transform.hasChanged)
        {
            _currentSample = 0;
            transform.hasChanged = false;
        }
    }
}
