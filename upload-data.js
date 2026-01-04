#!/usr/bin/env node

/**
 * Script Testing untuk SISTER PARKIR IoT
 * Mengupload data dummy ke Firebase Realtime Database
 * 
 * Gunakan: node upload-data.js
 */

const admin = require('firebase-admin');
const fs = require('fs');
const path = require('path');

// Download service account key dari Firebase Console terlebih dahulu
// Project Settings > Service Accounts > Generate New Private Key
const serviceAccountPath = path.join(__dirname, 'serviceAccountKey.json');

if (!fs.existsSync(serviceAccountPath)) {
    console.error('❌ ERROR: serviceAccountKey.json tidak ditemukan!');
    console.log('📝 Langkah-langkah:');
    console.log('1. Buka Firebase Console > https://console.firebase.google.com/');
    console.log('2. Pilih project: siste-parkir-iot');
    console.log('3. Buka Project Settings > Service Accounts');
    console.log('4. Klik "Generate New Private Key"');
    console.log('5. Simpan file JSON ke folder project dengan nama: serviceAccountKey.json');
    console.log('6. Jalankan script ini lagi: node upload-data.js');
    process.exit(1);
}

// Load database dummy
const databasePath = path.join(__dirname, 'database_dummy.json');
const dummyData = JSON.parse(fs.readFileSync(databasePath, 'utf8'));

// Initialize Firebase Admin SDK
const serviceAccount = require(serviceAccountPath);

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: 'https://siste-parkir-iot-default-rtdb.firebaseio.com'
});

const db = admin.database();

console.log('⏳ Sedang mengunggah data ke Firebase...');

db.ref('/').set(dummyData)
    .then(() => {
        console.log('✅ Berhasil! Data dummy berhasil diupload ke Firebase');
        console.log('\n📊 Data yang diupload:');
        console.log(`   - Users: ${Object.keys(dummyData.users).length} user`);
        console.log(`   - Logs: ${Object.keys(dummyData.logs).length} log`);
        console.log('\n🔗 Lihat di Firebase Console:');
        console.log('   https://console.firebase.google.com/project/siste-parkir-iot/database');
        process.exit(0);
    })
    .catch((error) => {
        console.error('❌ Error:', error.message);
        process.exit(1);
    });
