let health = 100;
let level = 1;
let petName = "";
let accounts = [];

function namePet() {
    petName = document.getElementById("petName").value;
    if (petName) {
        document.getElementById("petNameDisplay").textContent = `Hello, I am ${petName}!`;
        document.getElementById("defendimals").style.display = "block";
        document.getElementById("stats").style.display = "block";  // Show stats after naming pet
        document.getElementById("petName").style.display = "none";
        document.querySelector("button[onclick='namePet()']").style.display = "none";
    } else {
        alert("Please enter a name for your pet!");
    }
}

function updateStats() {
    document.getElementById('health').textContent = health;
    document.getElementById('level').textContent = level;
}

function displayAccounts() {
    const accountsList = document.getElementById("accounts");
    accountsList.innerHTML = ""; // Clear previous list

    accounts.forEach(account => {
        const listItem = document.createElement("li");
        listItem.textContent = `Platform: ${account.platform}, Username: ${account.username}, Password Strength: ${getStrengthMessage(calculatePasswordStrength(account.password))}, 2FA: ${account.twoFactorAuth ? 'Enabled' : 'Disabled'}`;
        accountsList.appendChild(listItem);
    });
}

// Function to add an account with enforced password strength and 2FA requirement
function addAccount() {
    const platform = document.getElementById("platform").value;
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const twoFactorAuth = document.getElementById("twoFactorAuth").checked;

    if (platform && username && password) {
        const passwordStrength = calculatePasswordStrength(password);
        if (passwordStrength < 3) { // Enforce minimum strength requirement
            document.getElementById("feedback").textContent = "Password too weak. Please enter a stronger password.";
        } else if (!twoFactorAuth) {
            document.getElementById("feedback").textContent = "Two-Factor Authentication is required for added security.";
        } else {
            accounts.push({ platform, username, password, twoFactorAuth });
            document.getElementById("feedback").textContent = `Account for ${platform} added successfully.`;
            displayAccounts(); // Update account list display
        }
    } else {
        document.getElementById("feedback").textContent = "Please enter platform, username, and password.";
    }
}

// Function to check password strength
function checkPasswordStrength() {
    const platform = document.getElementById("platform").value;
    const account = accounts.find(acc => acc.platform === platform);
    if (account) {
        const score = calculatePasswordStrength(account.password);
        const strength = getStrengthMessage(score);
        document.getElementById("feedback").textContent = `Password strength for ${platform}: ${strength}`;
    } else {
        document.getElementById("feedback").textContent = `No account found for ${platform}.`;
    }
}

// Password strength calculation
function calculatePasswordStrength(password) {
    let score = 0;
    if (password.length >= 8) score++;
    if (/[a-z]/.test(password)) score++;
    if (/[A-Z]/.test(password)) score++;
    if (/\d/.test(password)) score++;
    if (/[@$!%*?&]/.test(password)) score++;
    return score;
}

function getStrengthMessage(score) {
    if (score >= 4) return "Strong";
    if (score === 3) return "Moderate";
    return "Weak";
}

// Function to remove an account
function removeAccount() {
    const platform = document.getElementById("platform").value;
    const index = accounts.findIndex(acc => acc.platform === platform);
    if (index !== -1) {
        accounts.splice(index, 1);
        document.getElementById("feedback").textContent = `Account for ${platform} removed.`;
        displayAccounts(); // Update account list display
    } else {
        document.getElementById("feedback").textContent = `No account found for ${platform}.`;
    }
}

// Function to check URL safety
async function checkWebsite() {
    const website = document.getElementById("website").value;
    const feedback = document.getElementById("feedback");

    if (!website) {
        feedback.textContent = "Please enter a URL.";
        return;
    }

    try {
        const isUnsafe = await checkWebsiteSafety(website);
        if (isUnsafe) {
            feedback.textContent = `Warning: ${website} might be unsafe!`;
            health -= 10;
        } else {
            feedback.textContent = `${website} seems safe!`;
            health += 5;
            level++;
        }
    } catch (error) {
        feedback.textContent = "Error checking website safety.";
        console.error("API error:", error);
    }

    updateStats();
}


// Function to check website safety using Google Safe Browsing API
async function checkWebsiteSafety(url) {
    const apiKey = 'AIzaSyBQ6A1Opyx0Res0OZo_NJBfn7GTkzUtHBw'; // replace with your API key
    const safeBrowsingUrl = `https://safebrowsing.googleapis.com/v4/threatMatches:find?key=${apiKey}`;
    
    const body = {
        client: {
            clientId: "cyberPetApp",
            clientVersion: "1.0"
        },
        threatInfo: {
            threatTypes: ["MALWARE", "SOCIAL_ENGINEERING"],
            platformTypes: ["ANY_PLATFORM"],
            threatEntryTypes: ["URL"],
            threatEntries: [{ url }]
        }
    };

    try {
        const response = await fetch(safeBrowsingUrl, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body)
        });

        const result = await response.json();
        // If result contains matches, the URL is unsafe
        return result.matches ? true : false;
    } catch (error) {
        console.error("Error in Safe Browsing API:", error);
        throw error;
    }
}

