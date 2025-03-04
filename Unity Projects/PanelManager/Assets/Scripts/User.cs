using System.Collections.Generic;
using System.Linq;

//Base data per user
[System.Serializable]
public struct UserData {
    public string Name;
    public float MaxBrightness;
    public float MinBrightness;
    public float MaxVolume;
    public float MinVolume;

    public UserData() {
        Name = "NULL";
        MaxBrightness = 100;
        MinBrightness = 0;
        MaxVolume = 100;
        MinVolume = 0;
    }

    public UserData(string Name, float MaxBrightness = 100, float MinBrightness = 0, float MaxVolume = 100, float MinVolume = 0) {
        this.Name = Name;
        this.MaxBrightness = MaxBrightness;
        this.MinBrightness = MinBrightness;
        this.MaxVolume = MaxVolume;
        this.MinVolume = MinVolume;
    }

    readonly public string FormatArgsForArduino() { return Name + "," + MaxBrightness.ToString() + "," + MinBrightness.ToString() + "," + MaxVolume.ToString() + "," + MinVolume.ToString(); }
};


//Class for all of the users that will be saved/stored on the program
[System.Serializable]
public class Users {
    public List<UserData> allUsers;

    public Users() { allUsers = new(); AddUser(new UserData()); }

    public Users (List<UserData> users) { allUsers = users; }

    public void AddUser(UserData user) {
        allUsers.Add(user);
        allUsers = allUsers.OrderBy(x => x.Name).ToList();
    }

    public void RemoveUser(UserData user) { allUsers.Remove(user); }
}