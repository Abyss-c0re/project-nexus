# Join mesh (device recycle)

1. Install `nanobot` peer (Linux binary or Android foreground service).  
2. Generate **local** peer secret on device (never commit).  
3. Publish SMX heartbeat: `alive`, `arch`, `purpose=unassigned`, `integrity=ok`.  
4. Grokium `hive deploy` assigns purpose via commander-sealed plate.  
5. Node executes only SMX-lawful actions; prose channels stay local UI.

## Reject

- Uploading contacts, photos, SMS, accounts  
- Kinect / banned capture stacks  
- Cloud identity binding as a join requirement  
