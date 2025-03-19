using UnityEngine;
using System.IO;
using TMPro;
using System.Linq;

public class UserManager : MonoBehaviour {
    string filePath;
    static UserManager _Instance;
    public Users users;
    public bool initialized = false;
    public static UserManager Instance {
        get {
            if (_Instance == null) {
                GameObject go = GameObject.Find("Managers");
                DontDestroyOnLoad(go);
                _Instance = go.AddComponent<UserManager>();
            }
            return _Instance;
        }
    }

    public void Initialize() {
        filePath = Application.persistentDataPath + "/users.json";
        if (!File.Exists(filePath)) {
            users = new();
            Save();
        }
        else
            users = Load();
        initialized = true;
    }

    public void AddUser(UserData user) {
        users.AddUser(user);
        Save();
    }

    public void RemoveUser(UserData user) {
        users.RemoveUser(user);
        Save();
    }

    public void Save() { File.WriteAllText(filePath, JsonUtility.ToJson(users, true)); }

    public void SaveUser(UserData user) {
        int index = users.allUsers.FindIndex(x => x.Name == user.Name);
        if (index != -1) {
            users.allUsers[index] = user;
            Save();
        }
    }

    public Users Load() { return JsonUtility.FromJson<Users>(File.ReadAllText(filePath)); }
}