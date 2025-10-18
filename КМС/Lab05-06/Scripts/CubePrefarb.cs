using UnityEngine;

public class CubePrefarb : MonoBehaviour
{

    public GameObject cubePrefarb;
    public GameObject prefarb1;
    public float spawnHeight = 5f;
    public float planeSize = 3f;



	void Update()
    {

        if (Input.GetKeyDown(KeyCode.Q)) SpawnCube();

		if (Input.GetKeyDown(KeyCode.Space)) SpawnSphere();

		if (Input.GetKeyDown(KeyCode.W))
        {
            transform.Rotate(Vector3.forward * Time.deltaTime * 30f);
        }
    }

    void SpawnCube()
    {

		float x = Random.Range(-planeSize, planeSize);
		float z = Random.Range(-planeSize, planeSize);

		Vector3 spawnPosition = new Vector3(x, spawnHeight, z);

		GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube.transform.position = spawnPosition;

        cube.AddComponent<Rigidbody>();
	}

    void SpawnSphere()
    {
		float x = Random.Range(-planeSize, planeSize);
		float z = Random.Range(-planeSize, planeSize);

        Vector3 spawnPosition = new Vector3(x, spawnHeight, z);
        Instantiate(prefarb1, spawnPosition, Quaternion.identity);
    }
}
