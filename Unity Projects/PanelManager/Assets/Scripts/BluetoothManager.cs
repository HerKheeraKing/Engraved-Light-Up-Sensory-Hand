using UnityEngine;

public class BluetoothManager : MonoBehaviour {
    public static BluetoothManager Instance { get; private set; }

    void Awake() {
        if (Instance != null && Instance != this)
            Destroy(gameObject);
        else
            Instance = this;
    }

    public void EstablishConnection() {

    }
}
