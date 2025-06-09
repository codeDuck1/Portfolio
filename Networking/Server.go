package main

import (
    "encoding/json"
    "log"
    "net/http"
    "strconv"
    "sync"
    "time"
    "github.com/gin-gonic/gin"
    "github.com/gorilla/websocket"
)

// Struct used for data coming from website.
type UserInputData struct {
    ID       string  `json:"ID"`
    Username string  `json:"username"`
    Beta     float64 `json:"beta"`  // FB tilt
    Gamma    float64 `json:"gamma"` // LR tilt
}

// Struct used for data being read from Unity.
type DeviceOrientation struct {
    Username    string  `json:"username"`
    FB_Tilt     float64 `json:"fb_tilt"`
    LR_Tilt     float64 `json:"lr_tilt"`
    LastUpdate  int     `json:"last_update"`
}

// Thread-safe map to store user data (can be accessed concurrently by multiple threads without issues).
// i.e. Should be safe for concurrent rapid read/write.
var (
    devices      sync.Map // Store device data
    unityClients sync.Map // Store Unity client connections
)

// Upgrades http connection into websocket connection.
var upgrader = websocket.Upgrader{
    CheckOrigin: func(r *http.Request) bool {
        return true // Allow all origins for demo
    },
}

func main() {
    router := gin.Default()

    // Connect links to functions
    router.GET("/ws/web", handleWebConnection)    // For web clients
    router.GET("/ws/unity", handleUnityConnection) // For Unity clients

    startCleanup() // Inactive user server cleanup
    router.Run(":8080") // Attach the router to an http server and start it   
}

func handleWebConnection(c *gin.Context) {
    log.Printf("Received WebSocket connection request from: %s", c.Request.RemoteAddr)
    // Upgrade HTTP connection to WebSocket
    ws, err := upgrader.Upgrade(c.Writer, c.Request, nil)
    if err != nil {
        log.Printf("Failed to upgrade connection: %v", err)
        log.Printf("Headers: %v", c.Request.Header)
        return
    }
    log.Printf("Successfully upgraded connection to WebSocket")
    defer ws.Close()

    for {
        // Read messages from web client
        var inputData UserInputData
        err := ws.ReadJSON(&inputData)
        if err != nil {
            if websocket.IsUnexpectedCloseError(err, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
                log.Printf("WebSocket error: %v", err)
            }
            break
        }

        // Convert UserInputData to DeviceOrientation
        orientation := DeviceOrientation{
            Username:   inputData.Username,
            FB_Tilt:   inputData.Beta,
            LR_Tilt:   inputData.Gamma,
            LastUpdate: time.Now().Second(),
        }
        devices.Store(inputData.ID, orientation)

        // Broadcast to all Unity clients
        broadcastToUnity()
    }
}

func handleUnityConnection(c *gin.Context) {
    ws, err := upgrader.Upgrade(c.Writer, c.Request, nil)
    if err != nil {
        log.Printf("Failed to upgrade Unity connection: %v", err)
        return
    }
    // Defer is called when function exits, which in this case would be
    // when the websocket connection is closed in Unity.
    defer ws.Close() // Closes WS connection if necessary

    // Store Unity client connection
    unityClients.Store(ws, true)
    defer unityClients.Delete(ws) // Remove client from connected list

    // Send initial state
    broadcastToUnity()

    // Keep connection active 
    for {
        _, _, err := ws.ReadMessage()
        if err != nil {
            if websocket.IsUnexpectedCloseError(err, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
                log.Printf("Unity WebSocket error: %v", err)
            }
            break
        }
    }
}

func broadcastToUnity() {
    // Convert sync.Map to regular map to send to Unity via JSON.
    deviceData := make(map[int]DeviceOrientation)
    devices.Range(func(key, value interface{}) bool {
        if id, err := strconv.Atoi(key.(string)); err == nil {
            deviceData[id] = value.(DeviceOrientation)
        }
        return true
    })

    // Convert dictionary into JSON 
    data, err := json.Marshal(deviceData)
    if err != nil {
        log.Printf("Error marshaling data: %v", err)
        return
    }
    log.Printf("Sending data: %s", string(data))

    // Send JSON to all Unity clients
    unityClients.Range(func(key, value interface{}) bool {
        ws := key.(*websocket.Conn)
        if err := ws.WriteMessage(websocket.TextMessage, data); err != nil {
            log.Printf("Error sending to Unity client: %v", err)
            unityClients.Delete(key)
        }
        return true
    })
}

// Time to wait until deleting user from server
const waitTillRemove int = 10

func startCleanup() {
    ticker := time.NewTicker(10 * time.Second) // Ticks every 10 seconds
    go func() { // runs concurrently with rest of code using go
        for range ticker.C {  // infinite loop that waits for ticks
            currentSecond := time.Now().Second()
            devices.Range(func(key, value interface{}) bool {
                if orientation, ok := value.(DeviceOrientation); ok {
                    // Wraparound handling
                    timeDiff := currentSecond - orientation.LastUpdate // current second in realtime minute - last second in realtime minute
                    if timeDiff < 0 {
                        timeDiff += 60  // Add 60 if we wrapped around
                    }
                    // Delete if inactive for set time
                    if timeDiff > waitTillRemove {
                        devices.Delete(key)
                    }
                }
                return true
            })
        }
    }()
}