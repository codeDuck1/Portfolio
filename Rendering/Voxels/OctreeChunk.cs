using UnityEngine;
using NativeTrees;
using Unity.Collections;
using Unity.Mathematics;

// Initialize octree chunk and setup buffers between compute, vertex, and fragment shaders.
// Sends updated leaf node data to the compute shader for processing into transform matrices 
// with a color based on node depth. Then, draw call done here!
public class OctChunk2 : MonoBehaviour
{
    private NativeOctree<SplitData> octree;
    private Bounds chunkBounds;

    [SerializeField] int scale;
    [SerializeField] private Mesh instanceMesh;
    [SerializeField] private Material instanceMaterial;

    public int maxTreeDepth = 4;

    void Awake()
    {   
        // Place bottom of the octree at y=0
        chunkBounds = new Bounds(new Vector3(0, 4, 0), new Vector3(scale, scale, scale));
    
        // Create an AABB from the bounds for the octree
        AABB octreeBounds = new AABB( new float3(chunkBounds.min), new float3(chunkBounds.max));

        // Create octree, 1 object per node with max depth
        // Keep in memory until manually disposed of.
        octree = new NativeOctree<SplitData>(octreeBounds, 1, maxTreeDepth, Allocator.Persistent);
        
        // Must do initial insert to render correctly
        SplitData rootNode = new SplitData{};
        octree.Insert(rootNode, octreeBounds);
    }

    void Update()
    {
        RenderNodes();
    }
    
    void RenderNodes()
    {
        if (leavesBuffer == null || transBuffer == null || argsBuffer == null)
         return;
            
        // Calculate how many thread groups to dispatch
        int numLeaves = (int)_args[1]; // Get the instance count from args buffer
        int threadGroupsX = (numLeaves + 255) / 256; // For 256 threads

        cshader.Dispatch(kernelHandle, threadGroupsX, 1, 1);
        
        // Draw the instances using the transformation matrices
        Graphics.DrawMeshInstancedIndirect(
            instanceMesh,
            0,
            instanceMaterial,
            new Bounds(Vector3.zero, new Vector3(scale, scale, scale)),
            argsBuffer
        );
    }

    int kernelHandle; // Function that will process leaf nodes
    int bufferStride = sizeof(uint) +        // NodeId
                  sizeof(float) * 3 +    // Center 
                  sizeof(float) * 3 +    // Extents 
                  sizeof(int) +          // ObjectCount
                  sizeof(int);           // Depth


    
    [SerializeField] ComputeShader cshader;
    ComputeBuffer leavesBuffer;
    ComputeBuffer transBuffer;
    // Instance data required for instanced rendering
    ComputeBuffer argsBuffer; 
    private readonly uint[] _args = { 0, 0, 0, 0, 0 }; 

    void Start()
    {
        kernelHandle = cshader.FindKernel("CSMain");
        cshader.SetInt("maxNodeDepth", maxTreeDepth);
        
        // Used for coloring each cube
        Vector4 baseColorValue = new Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        cshader.SetVector("baseColor", baseColorValue);
        Vector4 depthColorValue = new Vector4(0.5f, 0.0f, 0.8f, 1.0f);
        cshader.SetVector("depthColorFactor", depthColorValue);

        // Size of each leaf node
        argsBuffer = new ComputeBuffer(1, _args.Length * sizeof(uint), ComputeBufferType.IndirectArguments);

        UpdateBuffers();
    
    }


    public void UpdateBuffers()
    {
        leavesBuffer?.Release();
        transBuffer?.Release();
        
        int numLeaves = 0;

        // Setup the buffer with new collection of leaf nodes
        using (var leafNodes = new NativeList<LeafNode>(Allocator.Temp))
        {
            octree.GetLeafNodes(leafNodes, maxTreeDepth);
            numLeaves = leafNodes.Length;

            // Empty buffer that compute shader will write to, creating matricies for vertex shader to read. 
            transBuffer = new ComputeBuffer(numLeaves, sizeof(float) * 16 + sizeof(float) * 4);

            // Set leaf node data to buffer, responsible for creating transforms.
            leavesBuffer = new ComputeBuffer(numLeaves, bufferStride);
            leavesBuffer.SetData(leafNodes.ToArray(Allocator.Temp));
            cshader.SetInt("currLeafTotal", numLeaves);
        }

        // Set function in compute to handle input data from leavesBuffer and write to transformbuffer
        cshader.SetBuffer(kernelHandle, "leavesBuffer", leavesBuffer);
        cshader.SetBuffer(kernelHandle, "transformBuffer", transBuffer);
        // Set material (that has vertex & frag shaders attached) to use transformBuffer which has been wrote to by compute shader
        instanceMaterial.SetBuffer("transformBuffer", transBuffer);

        // Verts
        _args[0] = instanceMesh.GetIndexCount(0);
        _args[1] = (uint) numLeaves;
        _args[2] = instanceMesh.GetIndexStart(0);
        _args[3] = instanceMesh.GetBaseVertex(0);
        argsBuffer.SetData(_args);
    }

    public NativeOctree<SplitData> GetChunkOctree()
    {
        return octree;
    }

    void OnDestroy()
    {
        octree.Dispose();
        leavesBuffer?.Release();
        transBuffer?.Release();
        argsBuffer?.Release();
    }
}






