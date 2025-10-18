using UnityEngine;
using UnityEngine.EventSystems;

public class CubeClicker : MonoBehaviour, IPointerClickHandler
{
	public void OnPointerClick(PointerEventData eventData)
	{
		float red = Random.Range(0.0f, 1.0f);
		float green = Random.Range(0.0f, 1.0f);
		float blue = Random.Range(0.0f, 1.0f);

		Color col1 = new Color(red, green, blue);
		gameObject.GetComponent<Renderer>().material.color = col1;
	}


}
